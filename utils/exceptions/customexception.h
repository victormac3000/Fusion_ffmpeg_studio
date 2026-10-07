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
        user_message(userMessage),
        user_message_formatted(userMessage + ".")
    {
        if (!silent) qWarning() << typeid(this).name() << "exception was thrown:" << exceptionMessage;
    }

    const std::string& userMessage() const noexcept
    {
        return user_message.empty() ? user_message : user_message_formatted;
    }

private:
    std::string user_message;
    std::string user_message_formatted;
};

#endif // CUSTOMEXCEPTION_H
