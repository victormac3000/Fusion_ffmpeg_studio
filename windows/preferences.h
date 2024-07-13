#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QQuickItem>

#include "utils/dialogs.h"
#include "utils/settings.h"

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
    void handleRequestEncoders(QString codec);
    void handleRequestVideoFormats(QString codec);
    void handleSave(QString data, QString type);

private:
    Ui::Preferences *ui;
    QSettings settings;
    QString errorMsg;
    QQuickItem *preferencesGeneralArea, *preferencesRenderingArea;


    bool copyAppData(QString path);
};

#endif // PREFERENCES_H
