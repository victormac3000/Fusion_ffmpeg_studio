#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QVector>

class WorkerThread : public QThread {
    Q_OBJECT

public:
    WorkerThread(QObject* parent = nullptr)
        : QThread(parent) {}

protected:
    void run() override {
        qDebug() << "Hello world from thread id" << QThread::currentThreadId();
    }
};

#endif // WORKERTHREAD_H
