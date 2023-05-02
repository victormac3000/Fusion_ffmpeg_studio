#ifndef DIALOGS_H
#define DIALOGS_H

#include <QString>
#include <QMessageBox>

class Dialogs
{
public:
    static void ok(QString message);
    static void warning(QString message);
    static void critical(QString message);
};

#endif // DIALOGS_H
