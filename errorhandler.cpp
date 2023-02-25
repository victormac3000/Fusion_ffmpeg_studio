#include "errorhandler.h"

ErrorHandler::ErrorHandler(QObject *parent)
    : QObject{parent}
{
    box = new QMessageBox;
}

void ErrorHandler::warning(QString err)
{
    box->setWindowTitle("Concerning error");
    box->setIcon(QMessageBox::Warning);
    box->setText(err);
    box->exec();
}

void ErrorHandler::critical(QString err)
{
    box->setWindowTitle("Critical error");
    box->setIcon(QMessageBox::Critical);
    box->setText(err);
    box->exec();
}
