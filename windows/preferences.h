#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QQuickItem>

#include "utils/dialogs.h"

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
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
