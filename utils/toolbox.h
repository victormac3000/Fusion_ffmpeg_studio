#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QString>

class Toolbox
{
public:
    static QString toLocalPath(const QString& path);
    static QString toAbsolutePath(QString relativePath, QString referenceDirPath);
    static QString toRelativePath(QString absolutePath, QString referenceDirPath);
};

#endif // TOOLBOX_H
