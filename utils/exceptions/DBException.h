#ifndef DBEXCEPTION_H
#define DBEXCEPTION_H

#include "utils/exceptions/customexception.h"

class DBException : public CustomException
{
public:
    DBException(const std::string& exceptionMessage,
                     const std::string& userMessage = "",
                     bool silent = true)
        : CustomException(exceptionMessage, userMessage, silent)
    {}
};

#endif // DBEXCEPTION_H
