#include "dialogs.h"
#include "utils/exitcodes.h"

void Dialogs::ok(QString userMsg, QString logMsg)
{
    qDebug() << "Ok dialog displayed:" << (logMsg.isEmpty() ? userMsg : logMsg);
    QMessageBox box;
    box.setWindowTitle("Information");
    box.setIcon(QMessageBox::Information);
    box.setText(userMsg);
    box.exec();
}

void Dialogs::warning(QString userMsg, QString logMsg)
{
    qWarning() << "Warning dialog displayed:" << (logMsg.isEmpty() ? userMsg : logMsg);
    QMessageBox box;
    box.setWindowTitle("Concerning error");
    box.setIcon(QMessageBox::Warning);
    box.setText(userMsg);
    box.exec();
}

void Dialogs::critical(QString userMsg, QString logMsg)
{
    qCritical() << "Critical dialog displayed:" << (logMsg.isEmpty() ? userMsg : logMsg);
    QMessageBox box;
    box.setWindowTitle("Critical error");
    box.setIcon(QMessageBox::Critical);
    box.setText(userMsg);
    box.exec();
    QCoreApplication::exit(ERROR_CRITICAL);
    exit(ERROR_CRITICAL);
}

void Dialogs::criticalStart(QString logMsg)
{
    qCritical() << "Critical start error" << logMsg;
    QMessageBox box;
    box.setWindowTitle("Critical error");
    box.setIcon(QMessageBox::Critical);
    box.setText("There was an internal error starting the application\nCheck the logs for more information");
    box.exec();
    QCoreApplication::exit(ERROR_CRITICAL);
    exit(ERROR_CRITICAL);
}
