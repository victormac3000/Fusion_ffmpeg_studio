#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QWidget>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileDialog>

#include "errorhandler.h"

namespace Ui {
class Preferences;
}

class Preferences : public QWidget
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = nullptr);
    ~Preferences();

private slots:
    void changeWorkingDir();
    void changeRenderedDir();

private:
    Ui::Preferences *ui;
    ErrorHandler *error;
    QSettings settings;
    QString errorMsg;

    bool canWrite(QString path);
};

#endif // PREFERENCES_H
