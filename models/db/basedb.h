#ifndef BASEDB_H
#define BASEDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QString>

class BaseDB
{
public:
    explicit BaseDB(const QString &databasePath, const QString &queriesPath);
    virtual ~BaseDB();

    BaseDB(const BaseDB&) = delete;
    BaseDB& operator=(const BaseDB&) = delete;

protected:
    /*
     * Execute a SELECT query.
     */
    QVariantList query(const QString &sqlID, const QVariantMap &parameters = {});
    /*
     * Execute a query and return the QSqlQuery itself.
     */
    QSqlQuery prepare(const QString &sqlID, const QVariantMap &parameters = {});
    /*
     * Execute INSERT / UPDATE / DELETE / CREATE TABLE etc.
     */
    int execute(const QString &sqlID, const QVariantMap &parameters = {});

    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    QSqlDatabase& database();
    QString connectionName() const;
private:
    void open();

    QMap<QString,QString> queries;
    QString m_databasePath;
    QString m_connectionName;
    QSqlDatabase m_database;
};

#endif // BASEDB_H
