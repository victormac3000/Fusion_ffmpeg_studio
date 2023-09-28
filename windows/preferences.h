#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QWidget>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileDialog>

#include "utils/dialogs.h"

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
    void changeAppDataDir();

private:
    Ui::Preferences *ui;
    QSettings settings;
    QString errorMsg;

    bool copyAppData(QString path);
};

#endif // PREFERENCES_H
