#ifndef PREFERENCESCONTROLLER_H
#define PREFERENCESCONTROLLER_H

#include "controllers/basecontroller.h"

class PreferencesController : public BaseController
{
    Q_OBJECT
public:
    explicit PreferencesController(QObject *parent = nullptr);

    Q_INVOKABLE QString getAppDataPath() const;
    Q_INVOKABLE QString getDefaultProjectName() const;
    Q_INVOKABLE QVariantMap changeAppDataDir(QString proposedAppDataDir) const;

private:
};

#endif // PREFERENCESCONTROLLER_H
