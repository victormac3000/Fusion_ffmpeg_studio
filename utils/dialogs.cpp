#include "dialogs.h"

void Dialogs::ok(QString userMsg, QString logMsg)
{
    qDebug() << "Ok dialog displayed:" << (logMsg.isEmpty() ? userMsg : logMsg);
    QMessageBox *box = new QMessageBox;
    box->setWindowTitle("Information");
    box->setIcon(QMessageBox::Information);
    box->setText(userMsg);
    box->exec();
}

void Dialogs::warning(QString userMsg, QString logMsg)
{
    qWarning() << "Warning dialog displayed:" << (logMsg.isEmpty() ? userMsg : logMsg);
    QMessageBox *box = new QMessageBox;
    box->setWindowTitle("Concerning error");
    box->setIcon(QMessageBox::Warning);
    box->setText(userMsg);
    box->exec();
}

void Dialogs::critical(QString userMsg, QString logMsg)
{
    qCritical() << "Critical dialog displayed:" << (logMsg.isEmpty() ? userMsg : logMsg);
    QMessageBox *box = new QMessageBox;
    box->setWindowTitle("Critical error");
    box->setIcon(QMessageBox::Critical);
    box->setText(userMsg);
    box->exec();
    exit(1000);
}
