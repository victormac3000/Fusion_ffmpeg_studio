#include "basedb.h"

#include <QAtomicInteger>
#include <QDateTime>
#include <QThread>
#include <QSqlError>
#include <QSqlRecord>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonObject>

#include "utils/exceptions/DBException.h"

namespace
{
    QAtomicInteger<quint64> connectionCounter = 0;

    QString createConnectionName()
    {
        const quint64 counter =
            connectionCounter.fetchAndAddRelaxed(1);

        const quintptr threadId =
            reinterpret_cast<quintptr>(QThread::currentThreadId());

        return QStringLiteral("Database_%1_%2_%3")
            .arg(threadId)
            .arg(QDateTime::currentMSecsSinceEpoch())
            .arg(counter);
    }
}


BaseDB::BaseDB(const QString &databasePath, const QString &queriesPath)
    : m_databasePath(databasePath),
    m_connectionName(createConnectionName())
{
    QFile queriesFile(queriesPath);

    if (!queriesFile.open(QFile::ReadOnly)) {
        qWarning() << "Cannot read projectdb.json file";
    }

    QJsonParseError docError;
    QJsonDocument doc = QJsonDocument::fromJson(
        queriesFile.readAll(), &docError
    );

    if (docError.error) {
        qWarning() << "Cannot parse projectdb.json file";
    }

    for (QString& key: doc.object().keys()) {
        this->queries.insert(key, doc.object().value(key).toString());
    }

    open();
}


BaseDB::~BaseDB()
{
    if (m_database.isValid()) {
        if (m_database.isOpen()) {
            m_database.close();
        }

        m_database = QSqlDatabase();
    }

    QSqlDatabase::removeDatabase(m_connectionName);
}


void BaseDB::open()
{
    m_database =
        QSqlDatabase::addDatabase(
            QStringLiteral("QSQLITE"),
            m_connectionName
            );

    m_database.setDatabaseName(m_databasePath);


    if (!m_database.open()) {
        throw DBException(
            QStringLiteral(
                "Could not open SQLite database '%1': %2"
                )
                .arg(
                    m_databasePath,
                    m_database.lastError().text()
                    )
                .toStdString()
            );
    }

    QSqlQuery pragma(m_database);

    if (!pragma.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        throw DBException(
            QStringLiteral(
                "Could not enable SQLite foreign keys: %1"
                )
                .arg(pragma.lastError().text())
                .toStdString()
            );
    }

    pragma.exec(QStringLiteral("PRAGMA journal_mode = WAL"));
    pragma.exec(QStringLiteral("PRAGMA busy_timeout = 5000"));
}


QVariantList BaseDB::query(
    const QString &sqlID,
    const QVariantMap &parameters)
{
    QSqlQuery query(m_database);

    if (!query.prepare(this->queries.value(sqlID))) {
        throw DBException(
            QStringLiteral(
                "Could not prepare SQL query: %1\nSQL: %2"
                )
                .arg(
                    query.lastError().text(),
                    sqlID
                    )
                .toStdString()
            );
    }

    for (auto it = parameters.constBegin();
         it != parameters.constEnd();
         ++it)
    {
        query.bindValue(
            QStringLiteral(":") + it.key(),
            it.value()
            );
    }

    if (!query.exec()) {
        throw DBException(
            QStringLiteral(
                "Could not execute SQL query: %1\nSQL: %2"
                )
                .arg(
                    query.lastError().text(),
                    sqlID
                    )
                .toStdString()
            );
    }

    QVariantList result;

    while (query.next()) {
        QVariantMap row;

        const QSqlRecord record = query.record();

        for (int i = 0; i < record.count(); ++i) {
            row.insert(
                record.fieldName(i),
                query.value(i)
                );
        }

        result.append(row);
    }

    return result;
}


int BaseDB::execute(
    const QString &sqlID,
    const QVariantMap &parameters)
{
    QSqlQuery query(m_database);

    if (!query.prepare(this->queries.value(sqlID))) {
        throw DBException(
            QStringLiteral(
                "Could not prepare SQL query: %1\nSQL: %2"
                )
                .arg(
                    query.lastError().text(),
                    sqlID
                    )
                .toStdString()
            );
    }

    for (auto it = parameters.constBegin();
         it != parameters.constEnd();
         ++it)
    {
        query.bindValue(
            QStringLiteral(":") + it.key(),
            it.value()
            );
    }

    if (!query.exec()) {
        throw DBException(
            QStringLiteral(
                "Could not execute SQL statement: %1\nSQL: %2"
                )
                .arg(
                    query.lastError().text(),
                    sqlID
                    )
                .toStdString()
            );
    }

    return query.numRowsAffected();
}


QSqlQuery BaseDB::prepare(
    const QString &sqlID,
    const QVariantMap &parameters
    )
{
    QSqlQuery query(m_database);

    if (!query.prepare(this->queries.value(sqlID))) {
        throw DBException(
            QStringLiteral(
                "Could not prepare SQL query: %1\nSQL: %2"
                )
                .arg(
                    query.lastError().text(),
                    sqlID
                    )
                .toStdString()
            );
    }

    for (auto it = parameters.constBegin();
         it != parameters.constEnd();
         ++it)
    {
        query.bindValue(
            QStringLiteral(":") + it.key(),
            it.value()
            );
    }

    if (!query.exec()) {
        throw DBException(
            QStringLiteral(
                "Could not execute SQL query: %1\nSQL: %2"
                )
                .arg(
                    query.lastError().text(),
                    sqlID
                    )
                .toStdString()
            );
    }

    return query;
}


void BaseDB::beginTransaction()
{
    if (!m_database.transaction()) {
        throw DBException(
            QStringLiteral(
                "Could not begin SQLite transaction: %1"
                )
                .arg(m_database.lastError().text())
                .toStdString()
            );
    }
}


void BaseDB::commitTransaction()
{
    if (!m_database.commit()) {
        throw DBException(
            QStringLiteral(
                "Could not commit SQLite transaction: %1"
                )
                .arg(m_database.lastError().text())
                .toStdString()
            );
    }
}


void BaseDB::rollbackTransaction()
{
    if (!m_database.rollback()) {
        throw DBException(
            QStringLiteral(
                "Could not rollback SQLite transaction: %1"
                )
                .arg(m_database.lastError().text())
                .toStdString()
        );
    }
}


QSqlDatabase& BaseDB::database()
{
    return m_database;
}


QString BaseDB::connectionName() const
{
    return m_connectionName;
}