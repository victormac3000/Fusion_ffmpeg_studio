#ifndef SETTINGSEXCEPTION_H
#define SETTINGSEXCEPTION_H

#include <stdexcept>
#include <string>
#include <QDebug>

class SettingsException : public std::runtime_error
{
public:
    SettingsException(const std::string& exceptionMessage,
                      const std::string& userMessage = "",
                      bool silent = false)
        : std::runtime_error(exceptionMessage),
        user_message(userMessage)
    {
        if (!silent) qWarning() << typeid(this).name() << "exception was thrown:" << exceptionMessage;
    }

    const std::string& userMessage() const noexcept
    {
        return user_message;
    }

private:
    std::string user_message;
};

#endif // SETTINGSEXCEPTION_H