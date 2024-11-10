#ifndef I_TO_STRING_H
#define I_TO_STRING_H

#include <string>

class IToString
{
public:
    virtual std::string ToString() const = 0;
    virtual ~IToString() {};
};

#endif // I_TO_STRING_H

