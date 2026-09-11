#ifndef CUSTOMEXCEPTION_H
#define CUSTOMEXCEPTION_H

#include <stdexcept>
#include <string>
#include <QDebug>

class CustomException : public std::runtime_error
{
public:
    CustomException(const std::string& exceptionMessage,
                      const std::string& userMessage = "",
                      bool silent = false)
        : std::runtime_error(exceptionMessage),
        user_message(userMessage + ".")
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

#endif // CUSTOMEXCEPTION_H
