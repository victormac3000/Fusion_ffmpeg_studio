#ifndef COPIEREXCEPTION_H
#define COPIEREXCEPTION_H

#include <stdexcept>
#include <string>
#include <QDebug>

class CopierException : public std::runtime_error
{
public:
    CopierException(const std::string& exceptionMessage,
                      const std::string& userMessage = "")
        : std::runtime_error(exceptionMessage),
        user_message(userMessage)
    {
        qWarning() << typeid(this).name() << "exception was thrown:" << exceptionMessage;
    }

    const std::string& userMessage() const noexcept
    {
        return user_message;
    }

private:
    std::string user_message;
};

#endif // COPIEREXCEPTION_H
