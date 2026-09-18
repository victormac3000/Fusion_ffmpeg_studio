#ifndef LOADINGEXCEPTION_H
#define LOADINGEXCEPTION_H

#include "utils/exceptions/customexception.h"

class LoadingException : public CustomException
{
public:
    LoadingException(const std::string& exceptionMessage,
                        const std::string& userMessage = "",
                        bool silent = false)
        : CustomException(exceptionMessage, userMessage, silent)
    {}
};

#endif // LOADINGEXCEPTION_H
