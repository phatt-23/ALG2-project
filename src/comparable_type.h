#ifndef __COMPARABLE_TYPE_H
#define __COMPARABLE_TYPE_H

#include <concepts>

/// @brief Concept that defines requirements for comparable types.
/// 
/// A type that satisfies this concept must support the following comparison
/// operations: less than, greater than, less than or equal to, greater than 
/// or equal to, equality, and inequality. Each operation must return a value 
/// that is convertible to a boolean.
template <typename T>
concept Comparable = requires(const T& a, const T& b) {
    { a < b } -> std::convertible_to<bool>;   ///< Less than comparison.
    { a > b } -> std::convertible_to<bool>;   ///< Greater than comparison.
    { a <= b } -> std::convertible_to<bool>;  ///< Less than or equal to comparison.
    { a >= b } -> std::convertible_to<bool>;  ///< Greater than or equal to comparison.
    { a == b } -> std::convertible_to<bool>;  ///< Equality comparison.
    { a != b } -> std::convertible_to<bool>;  ///< Inequality comparison.
};

#endif // __COMPARABLE_TYPE_H
