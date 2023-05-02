#include "dialogs.h"

void Dialogs::ok(QString message)
{
    QMessageBox *box = new QMessageBox;
    box->setWindowTitle("Information");
    box->setIcon(QMessageBox::Information);
    box->setText(message);
    box->exec();
}

void Dialogs::warning(QString message)
{
    QMessageBox *box = new QMessageBox;
    box->setWindowTitle("Concerning error");
    box->setIcon(QMessageBox::Warning);
    box->setText(message);
    box->exec();
}

void Dialogs::critical(QString message)
{
    QMessageBox *box = new QMessageBox;
    box->setWindowTitle("Critical error");
    box->setIcon(QMessageBox::Critical);
    box->setText(message);
    box->exec();
}
