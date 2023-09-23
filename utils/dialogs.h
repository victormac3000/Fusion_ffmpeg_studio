#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDebug>
#include <QString>
#include <QMessageBox>

class Dialogs
{
public:
    static void ok(QString userMsg, QString logMsg = "");
    static void warning(QString userMsg, QString logMsg = "");
    static void critical(QString userMsg, QString logMsg = "");
};

#endif // DIALOGS_H
