#ifndef __COMPARABLE_H
#define __COMPARABLE_H

/// @brief A template structure defining an interface for comparable objects.
/// 
/// This structure provides virtual methods for comparison operations
/// between instances of type T. It is meant to be inherited by specific 
/// types that require comparison logic.
template<typename T>
struct IComparable {
    /// @brief Default constructor for IComparable.
    IComparable() = default;

    /// @brief Virtual destructor for IComparable.
    virtual ~IComparable() = default;

    /// @brief Compares the current instance with another instance of type T.
    /// 
    /// This method should be implemented by derived classes to provide
    /// specific comparison logic.
    /// @param rhs The instance to compare against.
    /// @return An integer indicating the comparison result:
    ///         - A negative value if the current instance is less than rhs,
    ///         - Zero if they are equal,
    ///         - A positive value if the current instance is greater than rhs.
    virtual int Compare(const T& rhs) const = 0;

    /// @brief Checks if the current instance is less than the specified instance.
    /// @param rhs The instance to compare against.
    /// @return True if the current instance is less than rhs; otherwise, false.
    bool Less(const T& rhs) const { return (Compare(rhs) < 0); }

    /// @brief Checks if the current instance is less than or equal to the specified instance.
    /// @param rhs The instance to compare against.
    /// @return True if the current instance is less than or equal to rhs; otherwise, false.
    bool LessEqual(const T& rhs) const { return (Compare(rhs) <= 0); }

    /// @brief Checks if the current instance is greater than the specified instance.
    /// @param rhs The instance to compare against.
    /// @return True if the current instance is greater than rhs; otherwise, false.
    bool Greater(const T& rhs) const { return (Compare(rhs) > 0); }

    /// @brief Checks if the current instance is greater than or equal to the specified instance.
    /// @param rhs The instance to compare against.
    /// @return True if the current instance is greater than or equal to rhs; otherwise, false.
    bool GreaterEqual(const T& rhs) const { return (Compare(rhs) >= 0); }

    /// @brief Checks if the current instance is equal to the specified instance.
    /// 
    /// @param rhs The instance to compare against.
    /// @return True if the current instance is equal to rhs; otherwise, false.
    bool Equal(const T& rhs) const { return (Compare(rhs) == 0); }
};

#endif // __COMPARABLE_H
