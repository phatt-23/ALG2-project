#ifndef __COMPARABLE_H
#define __COMPARABLE_H

template<typename T>
struct Comparable {
    Comparable<T>() = default;
    virtual ~Comparable<T>() = default;

    virtual int compare (T rhs) const = 0;
    bool less           (T rhs) const { return (compare(rhs) < 0)   ? true : false; }
    bool lessEqual      (T rhs) const { return (compare(rhs) <= 0)  ? true : false; }
    bool greater        (T rhs) const { return (compare(rhs) > 0)   ? true : false; }
    bool greaterEqual   (T rhs) const { return (compare(rhs) >= 0)  ? true : false; }
    bool equal          (T rhs) const { return (compare(rhs) = 0)   ? true : false; }
};

#endif//__COMPARABLE_H
