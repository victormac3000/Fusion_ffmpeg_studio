#include "remover.h"
#include "utils/exceptions/removerexception.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileInfo>

void Remover::cleanDirectory(const QString &directoryPath)
{
    QDir directory(directoryPath);

    if (!directory.exists()) {
        throw RemoverException(
            "Directory does not exist: "
                + directoryPath.toStdString(),
            "The directory to clean does not exist."
            );
    }

    if (!directory.isReadable()) {
        throw RemoverException(
            "Directory is not readable: "
                + directoryPath.toStdString(),
            "The directory to clean cannot be read."
            );
    }

    const QFileInfoList entries = directory.entryInfoList(
        QDir::AllEntries |
        QDir::Hidden |
        QDir::System |
        QDir::NoDotAndDotDot
        );

    for (const QFileInfo &entry : entries) {

        if (entry.isDir()) {

            QDir subDirectory(entry.absoluteFilePath());

            if (!subDirectory.removeRecursively()) {
                throw RemoverException(
                    "Could not remove directory: "
                        + entry.absoluteFilePath().toStdString());
            }

        } else {

            if (!QFile::remove(entry.absoluteFilePath())) {
                throw RemoverException(
                    "Could not remove file: "
                        + entry.absoluteFilePath().toStdString());
            }
        }
    }
}
