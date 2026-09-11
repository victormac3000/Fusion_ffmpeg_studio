#ifndef COPIEREXCEPTION_H
#define COPIEREXCEPTION_H

#include "utils/exceptions/customexception.h"

class CopierException : public CustomException
{
public:
    CopierException(const std::string& exceptionMessage,
                    const std::string& userMessage = "",
                    bool silent = false)
        : CustomException(exceptionMessage, userMessage, silent)
    {}
};

#endif // COPIEREXCEPTION_H
