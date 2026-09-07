#ifndef COPIER_H
#define COPIER_H

#include <QDir>
#include <QString>
#include <QStringList>

    class Copier
{
public:
    explicit Copier(bool rollbackOnError = true, QFlags<QDir::Filter> fileTypes = QDir::NoDotAndDotDot |
                                                                                  QDir::AllEntries |
                                                                                  QDir::Hidden |
                                                                                  QDir::System);

    void copy(const QString& sourcePath,
              const QString& destinationPath);
    QString toLocalPath(const QString& path) const;

private:
    void copyDirectory(const QDir& source,
                       const QDir& destination);
    void rollback();
    void recordCreatedFile(const QString& path);
    void recordCreatedDirectory(const QString& path);

    bool rollbackOnError;
    QFlags<QDir::Filter> fileTypes;

    QStringList m_createdFiles;
    QStringList m_createdDirectories;
};

#endif // COPIER_H