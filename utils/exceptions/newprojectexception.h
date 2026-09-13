#ifndef NEWPROJECTEXCEPTION_H
#define NEWPROJECTEXCEPTION_H

#include "utils/exceptions/customexception.h"

class NewProjectException : public CustomException
{
public:
    NewProjectException(const std::string& exceptionMessage,
                                   const std::string& userMessage = "",
                                   bool silent = false)
        : CustomException(exceptionMessage, userMessage, silent)
    {}
};

#endif // NEWPROJECTEXCEPTION_H
