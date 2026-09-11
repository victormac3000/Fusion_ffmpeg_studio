#ifndef SETTINGSEXCEPTION_H
#define SETTINGSEXCEPTION_H

#include "utils/exceptions/customexception.h"

class SettingsException : public CustomException
{
public:
    SettingsException(const std::string& exceptionMessage,
                    const std::string& userMessage = "",
                    bool silent = false)
        : CustomException(exceptionMessage, userMessage, silent)
    {}
};

#endif // SETTINGSEXCEPTION_H