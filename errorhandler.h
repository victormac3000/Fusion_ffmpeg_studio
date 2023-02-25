#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QObject>
#include <QMessageBox>

class ErrorHandler : public QObject
{
    Q_OBJECT
public:
    explicit ErrorHandler(QObject *parent = nullptr);
    void warning(QString err);
    void critical(QString err);

signals:

private:
    QMessageBox *box;

};

#endif // ERRORHANDLER_H
