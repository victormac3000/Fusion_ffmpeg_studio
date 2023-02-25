#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QFile>

class Worker : public QThread
{
    Q_OBJECT
public:
    Worker(QObject *parent = nullptr);

public slots:
    void checkDCIMStructure(QDir dcim);

signals:
    void checkDCIMStructureDone(bool ok, QString errorMsg);

private:

};

#endif // WORKER_H
