#ifndef MYQSYSINFO_H
#define MYQSYSINFO_H

#include <QSysInfo>
#include <QStringList>
#include <QDebug>
#include <QCryptographicHash>
#include <QFile>
#include <QElapsedTimer>

struct VolumeInfo {
    QString label;
    QString mountPath;
    QString deviceName;
    QString fileSystemType;
    bool isExternal;
};

class MyQSysInfo : public QSysInfo
{
public:
    static QString cpuName();
    static QStringList gpuNames();
    static QString motherboardId();
    static QByteArray hardwareId();
    static QList<VolumeInfo> mountedVolumes();

private:
#ifdef Q_OS_WIN
    static bool isDriveUSB(const std::wstring& mountPath);
#endif
#ifdef Q_OS_LINUX
    static std::string readFile(const std::string& path);
#endif
#ifdef Q_OS_MAC
    static QString getVolumeLabel(const char* deviceName);
#endif

};

#endif // MYQSYSINFO_H
