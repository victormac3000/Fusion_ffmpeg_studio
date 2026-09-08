#ifndef PREFERENCESCONTROLLER_H
#define PREFERENCESCONTROLLER_H

#include "controllers/basecontroller.h"

class PreferencesController : public BaseController
{
    Q_OBJECT
public:
    explicit PreferencesController(QObject *parent = nullptr);

    Q_INVOKABLE QString getAppDataPath() const;
    Q_INVOKABLE void changeAppDataDir(QString proposedAppDataDir);
    Q_INVOKABLE void resetAppDataDir();

    Q_INVOKABLE QString getDefaultProjectName() const;
    Q_INVOKABLE void setDefaultProjectName(QString defaultName);

private:

signals:
    void appDataPathChanged(QVariantMap results);
};

#endif // PREFERENCESCONTROLLER_H
