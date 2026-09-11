#ifndef TOOLBOXEXCEPTION_H
#define TOOLBOXEXCEPTION_H

#include "utils/exceptions/customexception.h"

class ToolboxException : public CustomException
{
public:
    ToolboxException(const std::string& exceptionMessage,
                      const std::string& userMessage = "",
                      bool silent = false)
        : CustomException(exceptionMessage, userMessage, silent)
    {}
};

#endif // TOOLBOXEXCEPTION_H
