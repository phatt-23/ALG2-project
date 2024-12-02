#ifndef I_TO_STRING_H
#define I_TO_STRING_H

#include <string>

/// @brief An interface for classes that can provide a string representation of themselves.
/// 
/// Classes implementing this interface must define the `ToString` method, 
/// which returns a string that represents the current object. This is useful
/// for logging, debugging, or any situation where a human-readable
/// representation of the object is needed.
class IToString
{
public:
    /// @brief Converts the current object to its string representation.
    /// 
    /// This method should be implemented by derived classes to provide
    /// a meaningful string representation of the object.
    /// @return A string representing the current object.
    virtual std::string ToString() const = 0; // Pure virtual function

    /// @brief Virtual destructor for IToString.
    /// 
    /// The destructor is declared virtual to ensure proper cleanup of
    /// derived classes when an object is deleted through a base class pointer.
    virtual ~IToString() {};
};

#endif // I_TO_STRING_H
