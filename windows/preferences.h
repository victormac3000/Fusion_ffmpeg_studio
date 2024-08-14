#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QQuickItem>
#include <QJsonObject>

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
    void handleSave(QString data, QString type);
    void handleCodecChanged();

private:
    Ui::Preferences *ui;
    QSettings settings;
    QString errorMsg;
    QQuickItem *preferencesGeneralArea, *preferencesRenderingArea;
    QQuickItem *codecsComboBox, *encodersComboBox, *formatsComboBox;

    bool copyAppData(QString path);
};

#endif // PREFERENCES_H
