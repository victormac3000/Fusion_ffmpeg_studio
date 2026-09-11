#ifndef PREFERENCESCONTROLLER_H
#define PREFERENCESCONTROLLER_H

#include "controllers/basecontroller.h"

#include <QJsValue>
#include <QMap>

class PreferencesController : public BaseController
{
    Q_OBJECT
public:
    explicit PreferencesController(QObject *parent = nullptr);

    // Getters

    Q_INVOKABLE QString getAppDataPath() const;
    Q_INVOKABLE QString getDefaultProjectName() const;
    Q_INVOKABLE QString getDefaultProjectPath() const;

    // Setters

    Q_INVOKABLE void setAppDataPath(const QJSValue& args,
                                    const QJSValue& outputCallback,
                                    const QJSValue& errorCallback);
    Q_INVOKABLE void setDefaultProjectName(QString newName);
    Q_INVOKABLE void setDefaultProjectPath(const QJSValue& args,
                                           const QJSValue& outputCallback,
                                           const QJSValue& errorCallback);

    // Resetters

    Q_INVOKABLE void resetAppDataPath(const QJSValue& args,
                                      const QJSValue& outputCallback,
                                      const QJSValue& errorCallback);
    Q_INVOKABLE void resetDefaultProjectPath(const QJSValue& args,
                                             const QJSValue& outputCallback,
                                             const QJSValue& errorCallback);

private:

};

#endif // PREFERENCESCONTROLLER_H
