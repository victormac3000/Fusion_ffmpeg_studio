#include "copier.h"
#include "utils/exceptions/copierexception.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QUrl>

Copier::Copier(bool rollbackOnError, QFlags<QDir::Filter> fileTypes)
    : rollbackOnError(rollbackOnError),
        fileTypes(fileTypes)
{}

void Copier::copy(const QString& sourcePath,
                  const QString& destinationPath)
{
    m_createdFiles.clear();
    m_createdDirectories.clear();

    const QString sourceLocalPath = toLocalPath(sourcePath);
    const QString destinationLocalPath = toLocalPath(destinationPath);

    QDir source(sourceLocalPath);
    QDir destination(destinationLocalPath);

    if (!source.exists()) {
        throw CopierException(
            QString("Source directory does not exist: %1")
                .arg(sourceLocalPath)
                .toStdString(),
            "The source directory does not exist."
            );
    }

    if (!source.isReadable()) {
        throw CopierException(
            QString("Source directory is not readable: %1")
                .arg(sourceLocalPath)
                .toStdString(),
            "The source directory cannot be read."
            );
    }

    if (!destination.exists()) {
        throw CopierException(
            QString("Destination directory does not exist: %1")
                .arg(destinationLocalPath)
                .toStdString(),
            "The destination directory does not exist."
            );
    }

    if (!QFileInfo(destinationLocalPath).isDir()) {
        throw CopierException(
            QString("Destination is not a directory: %1")
                .arg(destinationLocalPath)
                .toStdString(),
            "The selected destination is not a directory."
            );
    }

    if (!QFileInfo(destinationLocalPath).isWritable()) {
        throw CopierException(
            QString("Destination directory is not writable: %1")
                .arg(destinationLocalPath)
                .toStdString(),
            "The destination directory is not writable."
            );
    }

    try {
        copyDirectory(source, destination);
    }
    catch (...) {
        if (rollbackOnError) {
            rollback();
        }

        throw;
    }

    m_createdFiles.clear();
    m_createdDirectories.clear();
}

void Copier::copyDirectory(const QDir& source,
                           const QDir& destination)
{
    const QFileInfoList entries =
        source.entryInfoList(fileTypes);

    for (const QFileInfo& entry : entries) {

        const QString sourcePath = entry.absoluteFilePath();
        const QString destinationPath =
            destination.filePath(entry.fileName());

        /*
         * Do not overwrite anything that already exists.
         */
        if (QFileInfo::exists(destinationPath)) {
            throw CopierException(
                QString("Destination already exists: %1")
                    .arg(destinationPath)
                    .toStdString(),
                "A file or directory already exists in the destination."
                );
        }

        if (entry.isDir()) {

            /*
             * Create destination directory.
             */
            if (!QDir().mkdir(destinationPath)) {
                throw CopierException(
                    QString("Could not create directory: %1")
                        .arg(destinationPath)
                        .toStdString(),
                    "Could not create a directory while copying the files."
                    );
            }

            /*
             * Record it immediately so it can be removed if
             * something fails later.
             */
            recordCreatedDirectory(destinationPath);

            QDir sourceSubDirectory(sourcePath);
            QDir destinationSubDirectory(destinationPath);

            /*
             * Recursively copy its contents.
             */
            copyDirectory(
                sourceSubDirectory,
                destinationSubDirectory
                );
        }
        else if (entry.isFile()) {

            /*
             * Copy file.
             */
            if (!QFile::copy(sourcePath, destinationPath)) {
                throw CopierException(
                    QString("Could not copy file '%1' to '%2'")
                        .arg(sourcePath, destinationPath)
                        .toStdString(),
                    "Could not copy a file. View the logs for more information."
                    );
            }

            /*
             * Record it immediately after successful creation.
             */
            recordCreatedFile(destinationPath);
        }
        else {
            /*
             * Symlinks, devices, sockets, etc.
             *
             * We deliberately don't try to copy them because
             * QFile::copy() isn't appropriate for all of them.
             */
            throw CopierException(
                QString("Unsupported filesystem entry: %1")
                    .arg(sourcePath)
                    .toStdString(),
                "The source contains a filesystem entry that cannot be copied."
                );
        }
    }
}

void Copier::recordCreatedFile(const QString& path)
{
    m_createdFiles.append(path);
}

void Copier::recordCreatedDirectory(const QString& path)
{
    m_createdDirectories.append(path);
}

QString Copier::toLocalPath(const QString& path) const
{
    const QUrl url(path);

    if (url.isLocalFile()) {
        return url.toLocalFile();
    }

    if (QFileInfo(path).isAbsolute()) {
        return path;
    }

    throw CopierException(
        QString("Path is neither a local path nor a local file URL: %1")
            .arg(path)
            .toStdString(),
        "The selected path is not a valid local directory."
    );
}

void Copier::rollback()
{
    qWarning() << "Rolling back copy operation";

    /*
     * Delete files first.
     */
    for (const QString& file : std::as_const(m_createdFiles)) {
        if (!QFile::remove(file)) {
            qWarning()
            << "Could not remove copied file during rollback:"
            << file;
        }
    }

    /*
     * Directories must be removed in reverse order because
     * parent directories contain directories created later.
     */
    for (auto it = m_createdDirectories.crbegin();
         it != m_createdDirectories.crend();
         ++it) {

        QDir directory(*it);

        if (!directory.rmdir(*it)) {
            qWarning()
            << "Could not remove directory during rollback:"
            << *it;
        }
    }

    m_createdFiles.clear();
    m_createdDirectories.clear();
}