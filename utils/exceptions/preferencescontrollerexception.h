#ifndef PREFERENCESCONTROLLEREXCEPTION_H
#define PREFERENCESCONTROLLEREXCEPTION_H

#include "utils/exceptions/customexception.h"

class PreferencesControllerException : public CustomException
{
public:
    PreferencesControllerException(const std::string& exceptionMessage,
                      const std::string& userMessage = "",
                      bool silent = false)
        : CustomException(exceptionMessage, userMessage, silent)
    {}
};

#endif // PREFERENCESCONTROLLEREXCEPTION_H
