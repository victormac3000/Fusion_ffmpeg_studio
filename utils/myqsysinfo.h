#ifndef MYQSYSINFO_H
#define MYQSYSINFO_H

#include <QSysInfo>
#include <QStringList>
#include <QDebug>
#include <QCryptographicHash>
#include <iostream>
#include <QFile>
#include <QElapsedTimer>

#ifdef Q_OS_MAC
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#include <dxgi.h>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "wbemuuid.lib")
#endif

#ifdef Q_OS_LINUX
#include <fstream>
#endif

class MyQSysInfo : public QSysInfo
{
public:
    static QString cpuName();
    static QStringList gpuNames();
    static QString motherboardId();
    static QByteArray hardwareId();

private:
#ifdef Q_OS_LINUX
    static std::string readFile(const std::string& path);
#endif

};

#endif // MYQSYSINFO_H
