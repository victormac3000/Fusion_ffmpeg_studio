#include "toolbox.h"
#include "utils/exceptions/toolboxexception.h"

#include <QUrl>
#include <QFileInfo>


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
