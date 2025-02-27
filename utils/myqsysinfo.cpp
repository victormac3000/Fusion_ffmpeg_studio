#include "myqsysinfo.h"

QString MyQSysInfo::cpuName()
{
    QString cpuName = "Unknown CPU";

    #ifdef Q_OS_WIN
    HKEY hKey;
    char cpuNameArr[256];
    DWORD bufferSize = sizeof(cpuNameArr);

    LONG result = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0,
        KEY_READ,
        &hKey
        );

    if (result != ERROR_SUCCESS) {
        qWarning() << "Could not get CPU name. Failed to open registry key. Error code: " << result;
        return cpuName;
    }

    result = RegQueryValueExA(hKey,
        "ProcessorNameString",
        NULL,
        NULL,
        (LPBYTE) cpuNameArr,
        &bufferSize
    );

    if (result != ERROR_SUCCESS) {
        qWarning() << "Could not get CPU name. Failed to open registry key. Error code: " << result;
        RegCloseKey(hKey);
        return cpuName;
    }

    cpuName = cpuNameArr;

    RegCloseKey(hKey);
    #endif

    #ifdef Q_OS_MAC
    char buffer[128];
    size_t bufferlen = sizeof(buffer);

    if (sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferlen, nullptr, 0) == 0) {
        cpuName = QString(buffer);
    }
    #endif

    #ifdef Q_OS_LINUX
    std::ifstream cpuFile("/proc/cpuinfo");
    std::string line;

    if (!cpuFile.is_open()) {
        qWarning() << "Cannot get CPU name. Cannot open CPU info file on /proc/cpuinfo";
        return cpuName;
    }

    while (std::getline(cpuFile, line)) {
        if (line.find("model name") != std::string::npos) {
            cpuName = QString::fromStdString(line.substr(line.find(":") + 2));
            break;
        }
    }

    cpuFile.close();
    #endif

    return cpuName;
}

QStringList MyQSysInfo::gpuNames()
{
    QStringList gpuNames;

    #ifdef Q_OS_MAC
    // Nothing because in macos the decoder is always videotoolbox
    #endif

    #ifdef Q_OS_WIN
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    if (FAILED(hr)) {
        QString type = "Unknown error";
        if (hr == S_FALSE) type = "The COM library has already initialized in this process";
        if (hr == RPC_E_CHANGED_MODE) type = "A previous call to CoInitializeEx specified the concurrency model for this thread as multithread apartment (MTA). This could also indicate that a change from neutral-threaded apartment to single-threaded apartment has occurred.";
        qWarning() << "Could not list GPUs: Failed to initialize COM library. Error: " << type;
        return gpuNames;
    }

    IDXGIFactory* pFactory = nullptr;
    HRESULT hrIDXGIFactory = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**) &pFactory);
    if (FAILED(hrIDXGIFactory)) {
        qWarning() << "Could not list GPUs: Failed to create DXGI Factory. Return value: " << hrIDXGIFactory;
        CoUninitialize();
        return gpuNames;
    }

    UINT i = 0;
    IDXGIAdapter* pAdapter = nullptr;
    while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC desc;
        char gpuName[1024];
        char defChar = ' ';

        pAdapter->GetDesc(&desc);

        int result = WideCharToMultiByte(
            CP_ACP,
            WC_COMPOSITECHECK | WC_DEFAULTCHAR,
            desc.Description,
            -1,
            gpuName,
            sizeof(gpuName),
            &defChar,
            nullptr
        );

        if (result == 0) {
            DWORD lastError = GetLastError();

            if (lastError == ERROR_INSUFFICIENT_BUFFER) {
                qWarning() << "Could not list GPUs: A supplied buffer size was not large enough, or it was incorrectly set to NULL.";
                return gpuNames;
            }

            if (lastError == ERROR_INVALID_FLAGS) {
                qWarning() << "Could not list GPUs: The values supplied for flags were not valid.";
                return gpuNames;
            }


            if (lastError == ERROR_INVALID_PARAMETER) {
                qWarning() << "Could not list GPUs: Any of the parameter values was invalid.";
                return gpuNames;
            }


            if (lastError == ERROR_NO_UNICODE_TRANSLATION) {
                qWarning() << "Could not list GPUs: Invalid Unicode was found in a string.";
                return gpuNames;
            }

            qWarning() << "Could not list GPUs: Unknown error in the wchar conversion.";
            return gpuNames;
        }

        gpuNames.append(gpuName);

        pAdapter->Release();
        i++;
    }

    pFactory->Release();
    CoUninitialize();
    #endif

    #ifdef Q_OS_LINUX
    QElapsedTimer tmr;
    tmr.start();
    std::string basePath = "/sys/class/drm/";

    for (const auto& entry : std::filesystem::directory_iterator(basePath)) {
        std::string path = entry.path();
        if (path.find("card") != std::string::npos && path.find("render") == std::string::npos) {
            std::string devicePath = path + "/device";
            std::ifstream vendorFile(devicePath + "/vendor");
            std::ifstream deviceFile(devicePath + "/device");

            if (vendorFile.is_open() && deviceFile.is_open()) {
                std::string vendorID, deviceID;
                std::getline(vendorFile, vendorID);
                std::getline(deviceFile, deviceID);

                unsigned int vendor = std::stoul(vendorID, nullptr, 16);

                if (vendor == 0x10de) {
                    gpuNames.push_back("NVIDIA GPU");
                } else if (vendor == 0x1002) {
                    gpuNames.push_back("AMD GPU");
                } else if (vendor == 0x8086) {
                    gpuNames.push_back("Intel GPU");
                } else {
                    gpuNames.push_back("Unknown GPU");
                }
            }
        }
    }
    #endif

    return gpuNames;
}

QString MyQSysInfo::motherboardId()
{
    QString motherboardName = "Unknown motherboard";

    #ifdef Q_OS_WIN
    HRESULT hres;

    // Step 1: Initialize COM
    hres = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    if (FAILED(hres)) {
        QString error = "Unknown error";

        if (hres == E_INVALIDARG) error = "Invalid arguments";
        if (hres == E_OUTOFMEMORY) error = "Out of memory";
        if (hres == E_UNEXPECTED) error = "Unexpected error";
        if (hres == S_FALSE) error = "The COM library is already initialized in this thread";
        if (hres == RPC_E_CHANGED_MODE) error = "A previous call to CoInitializeEx specified the concurrency model for this thread as multithread apartment (MTA). This could also indicate that a change from neutral-threaded apartment to single-threaded apartment has occurred.";

        qWarning() << "Could not get motherboard name. Failed to initialize COM library. Error: "
                   << error;
        return motherboardName;
    }

    // Step 2: Set COM security levels
    hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM negotiates service
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
    );

    if (FAILED(hres) && hres != RPC_E_TOO_LATE) {
        qWarning() << "Could not get motherboard name. Failed to initialize security. Error code = "
                   << hres;
        CoUninitialize();
        return motherboardName;
    }

    // Step 3: Obtain the initial locator to WMI
    IWbemLocator *pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *)&pLoc);

    if (FAILED(hres)) {
        qWarning() << "Could not get motherboard name. Failed to create IWbemLocator object. Error code = "
                   << hres;
        CoUninitialize();
        return motherboardName;
    }

    // Step 4: Connect to WMI through the IWbemLocator::ConnectServer method
    IWbemServices *pSvc = NULL;

    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // WMI namespace
        NULL,                    // User name
        NULL,                    // User password
        0,                       // Locale
        NULL,                    // Security flags
        0,                       // Authority
        0,                       // Context object
        &pSvc                    // IWbemServices proxy
        );

    if (FAILED(hres)) {
        qWarning() << "Could not get motherboard name. Could not connect. Error code = "
                   << hres;
        pLoc->Release();
        CoUninitialize();
        return motherboardName;
    }

    // Step 5: Set security levels on the proxy
    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        NULL,                        // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL,                        // client identity
        EOAC_NONE                    // proxy capabilities
        );

    if (FAILED(hres)) {
        qWarning() << "Could not get motherboard name. Could not set proxy blanket. Error code = "
                   << hres;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return motherboardName;
    }

    // Step 6: Use the IWbemServices pointer to make requests of WMI
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT Manufacturer, Product FROM Win32_BaseBoard"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        qWarning() << "Could not get motherboard name. Query for motherboard info failed. Error code = "
                   << hres;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return motherboardName;
    }

    // Step 7: Get the data from the query
    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if (uReturn == 0) {
            break;
        }

        VARIANT vtProp;

        // Get the Manufacturer property
        HRESULT hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            char manufacturer[1024];
            char defChar = ' ';

            int result = WideCharToMultiByte(
                CP_ACP,
                WC_COMPOSITECHECK | WC_DEFAULTCHAR,
                vtProp.bstrVal,
                -1,
                manufacturer,
                sizeof(manufacturer),
                &defChar,
                nullptr
            );

            if (result == 0) {
                DWORD lastError = GetLastError();

                if (lastError == ERROR_INSUFFICIENT_BUFFER) {
                    qWarning() << "Could not get motherboard name: A supplied buffer size was not large enough, or it was incorrectly set to NULL.";
                    return motherboardName;
                }

                if (lastError == ERROR_INVALID_FLAGS) {
                    qWarning() << "Could not get  motherboard name: The values supplied for flags were not valid.";
                    return motherboardName;
                }


                if (lastError == ERROR_INVALID_PARAMETER) {
                    qWarning() << "Could not get  motherboard name: Any of the parameter values was invalid.";
                    return motherboardName;
                }


                if (lastError == ERROR_NO_UNICODE_TRANSLATION) {
                    qWarning() << "Could not get  motherboard name: Invalid Unicode was found in a string.";
                    return motherboardName;
                }

                qWarning() << "Could not get  motherboard name: Unknown error in the wchar conversion.";
                return motherboardName;
            }

            motherboardName = manufacturer;

            VariantClear(&vtProp);
        }

        // Get the Product property
        hr = pclsObj->Get(L"Product", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            char model[1024];
            char defChar = ' ';

            int result = WideCharToMultiByte(
                CP_ACP,
                WC_COMPOSITECHECK | WC_DEFAULTCHAR,
                vtProp.bstrVal,
                -1,
                model,
                sizeof(model),
                &defChar,
                nullptr
            );

            if (result == 0) {
                DWORD lastError = GetLastError();

                if (lastError == ERROR_INSUFFICIENT_BUFFER) {
                    qWarning() << "Could not get motherboard name: A supplied buffer size was not large enough, or it was incorrectly set to NULL.";
                    return motherboardName;
                }

                if (lastError == ERROR_INVALID_FLAGS) {
                    qWarning() << "Could not get  motherboard name: The values supplied for flags were not valid.";
                    return motherboardName;
                }


                if (lastError == ERROR_INVALID_PARAMETER) {
                    qWarning() << "Could not get  motherboard name: Any of the parameter values was invalid.";
                    return motherboardName;
                }


                if (lastError == ERROR_NO_UNICODE_TRANSLATION) {
                    qWarning() << "Could not get  motherboard name: Invalid Unicode was found in a string.";
                    return motherboardName;
                }

                qWarning() << "Could not get  motherboard name: Unknown error in the wchar conversion.";
                return motherboardName;
            }

            motherboardName.append(" ");
            motherboardName.append(model);

            VariantClear(&vtProp);
        }

        pclsObj->Release();
    }

    // Cleanup
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
    #endif

    #ifdef Q_OS_MAC
    size_t len = 0;
    sysctlbyname("hw.model", NULL, &len, NULL, 0);
    if (len) {
        char model[1024];
        sysctlbyname("hw.model", &model, &len, NULL, 0);
        motherboardName = model;
    }
    #endif

    #ifdef Q_OS_LINUX
    std::string basePath = "/sys/class/dmi/id/";
    std::string vendorFile = basePath + "board_vendor";
    std::string modelFile = basePath + "board_name";

    std::string vendor = readFile(vendorFile);
    std::string model = readFile(modelFile);

    motherboardName = QString::fromStdString(vendor + " " + model);
    #endif

    return motherboardName;
}

#ifdef Q_OS_LINUX
std::string MyQSysInfo::readFile(const std::string& path)
{
    std::ifstream file(path);
    std::string content;
    if (file.is_open()) {
        std::getline(file, content);
        file.close();
    } else {
        qWarning() << "Could not get motherboard name. Could not open file " << QString::fromStdString(path);
    }
    return content;
}
#endif

QByteArray MyQSysInfo::hardwareId()
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(motherboardId().toUtf8());
    hash.addData(cpuName().toUtf8());
    hash.addData(gpuNames().join("|").toUtf8());
    return hash.result();
}

QList<VolumeInfo> MyQSysInfo::mountedVolumes()
{
    QList<VolumeInfo> mountedVolumes;

    #ifdef Q_OS_WIN
    wchar_t driveStrings[256];
    DWORD driveCount = GetLogicalDriveStrings(sizeof(driveStrings) / sizeof(driveStrings[0]), driveStrings);

    if (driveCount == 0) {
        qWarning() << "Failed to retrieve drive strings";
        return {};
    }

    for (wchar_t* drive = driveStrings; *drive != '\0'; drive += 4) {
        VolumeInfo volumeInfo;
        volumeInfo.mountPath = QString::fromWCharArray(drive);

        UINT driveType = GetDriveType(drive);
        if (driveType == DRIVE_REMOVABLE) {
            volumeInfo.isExternal = true; // Automatically mark removable drives as external
        } else if (driveType == DRIVE_FIXED) {
            // Check if the drive is connected via USB
            volumeInfo.isExternal = isDriveUSB(drive);
        } else {
            volumeInfo.isExternal = false;
        }

        wchar_t volumeName[MAX_PATH + 1] = { 0 };
        wchar_t fileSystemName[MAX_PATH + 1] = { 0 };
        DWORD serialNumber = 0, maxComponentLen = 0, fileSystemFlags = 0;

        if (GetVolumeInformation(
                drive,
                volumeName,
                sizeof(volumeName) / sizeof(volumeName[0]),
                &serialNumber,
                &maxComponentLen,
                &fileSystemFlags,
                fileSystemName,
                sizeof(fileSystemName) / sizeof(fileSystemName[0])
                )) {
            volumeInfo.label = QString::fromWCharArray(volumeName);
            volumeInfo.fileSystemType = QString::fromWCharArray(fileSystemName);
        } else {
            volumeInfo.label = "Unknown";
            volumeInfo.fileSystemType = "Unknown";
        }

        wchar_t devicePath[MAX_PATH] = { 0 };
        if (QueryDosDevice(&drive[0], devicePath, sizeof(devicePath) / sizeof(devicePath[0])) != 0) {
            volumeInfo.deviceName = QString::fromWCharArray(devicePath);
        } else {
            volumeInfo.deviceName = "Unknown";
        }

        mountedVolumes.append(volumeInfo);
    }

    #endif

    #ifdef Q_OS_MAC
    struct statfs *mounts;
    int numMounts = getmntinfo(&mounts, MNT_NOWAIT);

    if (numMounts > 0) {
        for (int i=0; i<numMounts; i++) {
            QString path = mounts[i].f_mntonname;
            QString fsType = mounts[i].f_fstypename;
            QString deviceName = mounts[i].f_mntfromname;
            QString volumeLabel = getVolumeLabel(mounts[i].f_mntfromname);

            if (fsType == "devfs" || fsType == "autofs" ||
                fsType == "tmpfs" || fsType == "overlay" ||
                path.startsWith("/System") || path.startsWith("/private") || path == "/") {
                continue;
            }

            VolumeInfo volumeInfo;
            volumeInfo.deviceName = deviceName;
            volumeInfo.label = volumeLabel;
            volumeInfo.fileSystemType = fsType;
            volumeInfo.mountPath = path;
            volumeInfo.isExternal = !(mounts[i].f_flags & MNT_LOCAL);

            if (volumeInfo.label.isEmpty()) {
                QString mountPathCopy = volumeInfo.mountPath;
                volumeInfo.label = mountPathCopy.replace("/Volumes/", "");
            }

            mountedVolumes.append(volumeInfo);
        }
    }
    #endif

    return mountedVolumes;
}

#ifdef Q_OS_WIN
bool MyQSysInfo::isDriveUSB(const std::wstring &mountPath)
{
    bool isUSB = false;
    HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_DISK, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDevInfo == INVALID_HANDLE_VALUE) return false;

    SP_DEVICE_INTERFACE_DATA deviceInterfaceData = {};
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &GUID_DEVINTERFACE_DISK, i, &deviceInterfaceData); ++i) {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetail(hDevInfo, &deviceInterfaceData, NULL, 0, &requiredSize, NULL);
        if (requiredSize == 0) continue;

        std::vector<BYTE> detailDataBuffer(requiredSize);
        SP_DEVICE_INTERFACE_DETAIL_DATA* deviceInterfaceDetailData = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA*>(detailDataBuffer.data());
        deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        SP_DEVINFO_DATA deviceInfoData = {};
        deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

        if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &deviceInterfaceData, deviceInterfaceDetailData, requiredSize, NULL, &deviceInfoData)) {
            TCHAR devicePath[MAX_PATH];
            if (SetupDiGetDeviceRegistryProperty(hDevInfo, &deviceInfoData, SPDRP_LOCATION_INFORMATION, NULL, (BYTE*)devicePath, sizeof(devicePath), NULL)) {
                if (_tcsstr(devicePath, _T("USB")) != NULL) {
                    isUSB = true;
                    break;
                }
            }
        }
    }
    SetupDiDestroyDeviceInfoList(hDevInfo);
    return isUSB;
}
#endif

#ifdef Q_OS_MAC
QString MyQSysInfo::getVolumeLabel(const char* deviceName)
{
    QString label;

    // Create a Disk Arbitration session
    DASessionRef session = DASessionCreate(kCFAllocatorDefault);
    if (session) {
        // Create a disk reference from the device name
        DADiskRef disk = DADiskCreateFromBSDName(kCFAllocatorDefault, session, deviceName);
        if (disk) {
            // Get disk information dictionary
            CFDictionaryRef diskInfo = DADiskCopyDescription(disk);
            if (diskInfo) {
                // Get the volume name from the dictionary
                CFStringRef volumeName = (CFStringRef)CFDictionaryGetValue(diskInfo, kDADiskDescriptionVolumeNameKey);
                if (volumeName) {
                    // Convert the volume name to a C++ string
                    char nameBuffer[256];
                    if (CFStringGetCString(volumeName, nameBuffer, sizeof(nameBuffer), kCFStringEncodingUTF8)) {
                        label = nameBuffer;
                    }
                }
                CFRelease(diskInfo);
            }
            CFRelease(disk);
        }
        CFRelease(session);
    }

    return label;
}
#endif
