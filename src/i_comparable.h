#ifndef __COMPARABLE_H
#define __COMPARABLE_H

template<typename T>
struct IComparable {
    IComparable<T>() = default;
    virtual ~IComparable<T>() = default;

    virtual int Compare (const T& rhs) const = 0;
    bool Less           (const T& rhs) const { return (Compare(rhs) < 0)  ? true : false; }
    bool LessEqual      (const T& rhs) const { return (Compare(rhs) <= 0) ? true : false; }
    bool Greater        (const T& rhs) const { return (Compare(rhs) > 0)  ? true : false; }
    bool GreaterEqual   (const T& rhs) const { return (Compare(rhs) >= 0) ? true : false; }
    bool Equal          (const T& rhs) const { return (Compare(rhs) = 0)  ? true : false; }
};

#endif//__COMPARABLE_H
