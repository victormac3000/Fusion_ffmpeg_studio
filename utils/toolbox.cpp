#include "toolbox.h"
#include "utils/exceptions/toolboxexception.h"

#include <QUrl>
#include <QFileInfo>
#include <QDir>

QString Toolbox::toLocalPath(const QString &path)
{
    const QUrl url(path);

    if (url.isLocalFile()) {
        return url.toLocalFile();
    }

    if (QFileInfo(path).isAbsolute()) {
        return path;
    }

    throw ToolboxException(
        QString("Path is neither a local path nor a local file URL: %1")
            .arg(path)
            .toStdString(),
        "The selected path is not a valid local directory."
    );
}

QString Toolbox::toAbsolutePath(QString relativePath, QString referenceDirPath)
{
    return QDir::cleanPath(
        QDir(referenceDirPath).absoluteFilePath(relativePath)
    );
}

QString Toolbox::toRelativePath(QString absolutePath, QString referenceDirPath)
{
    return QDir::toNativeSeparators(
        QDir(referenceDirPath).relativeFilePath(absolutePath)
    );
}


