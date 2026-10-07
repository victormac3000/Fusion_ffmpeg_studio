#ifndef PROJECTEXCEPTION_H
#define PROJECTEXCEPTION_H

#include "utils/exceptions/customexception.h"

class ProjectException : public CustomException
{
public:
    ProjectException(const std::string& exceptionMessage,
                     const std::string& userMessage = "",
                     bool silent = true)
        : CustomException(exceptionMessage, userMessage, silent)
    {}
};

#endif // PROJECTEXCEPTION_H
