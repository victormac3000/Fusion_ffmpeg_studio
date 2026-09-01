#include "aboutcontroller.h"

#include <QFile>
#include <QDebug>

AboutController::AboutController(QObject *parent)
    : BaseController{parent}
{

}

QString AboutController::readResourceFile(const QString &path) const
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Could not open file:" << path;
        return "";
    }

    return QString::fromUtf8(file.readAll());
}
