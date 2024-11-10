#ifndef __COMPARABLE_TYPE_H
#define __COMPARABLE_TYPE_H

#include <concepts>

template <typename T>
concept Comparable = requires(const T& a, const T& b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
    { a <= b } -> std::convertible_to<bool>;
    { a >= b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

#endif//__COMPARABLE_TYPE_H
