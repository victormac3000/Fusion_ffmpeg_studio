#ifndef REMOVEREXCEPTION_H
#define REMOVEREXCEPTION_H

#include "utils/exceptions/customexception.h"

class RemoverException : public CustomException
{
public:
    RemoverException(const std::string& exceptionMessage,
                                   const std::string& userMessage = "",
                                   bool silent = false)
        : CustomException(exceptionMessage, userMessage, silent)
    {}
};

#endif // REMOVEREXCEPTION_H
