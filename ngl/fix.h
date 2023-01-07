#ifndef FIX_H
#define FIX_H

#include <cstdint>
#include <cmath>
#include <limits>
#include <cstring>
#include <cstdio>
#include "extra_math.h"

template <unsigned int s, typename T=int_fast32_t> class Fix
{   
public:
    constexpr Fix() : value(0) {}
    template <unsigned int s2, typename T2> Fix(const Fix<s2, T2> f) : value((s2 > s) ? (f.value >> (s2 - s)) : (f.value << (s-s2))) {}
    constexpr Fix(const float v) : value(static_cast<T>(v * static_cast<float>(1<<s))) {}
    constexpr Fix(const unsigned int v) : value(v << s) {}
    constexpr Fix(const int v) : value(static_cast<unsigned int>(v) << s) {}
    constexpr Fix(const long int v) : value(static_cast<unsigned long int>(v) << s) {}
    constexpr operator float() const { return toFloat(); }
    constexpr operator int() const { return toInteger<int>(); }
    constexpr operator unsigned int() const { return toInteger<unsigned int>(); }
    constexpr operator uint16_t() const { return toInteger<uint16_t>(); }

    T round() const { T ret = value >> (s-1); return (ret>>1) + (ret&1); }
    constexpr T floor() const { return value >> s; }

    Fix<s,T> wholes() const { Fix<s,T> ret; ret.value = value & ~((1<<s)-1); return ret; }

    Fix<s,T> operator -() const { Fix<s,T> ret; ret.value = -value; return ret; }
    Fix<s,T>& operator +() const { return *this; }

    Fix<s,T>& operator ++() { value += 1 << s; return *this; }
    Fix<s,T>& operator --() { value -= 1 << s; return *this; }

    template <typename U> Fix<s,T> operator >>(const U other) const { Fix<s,T> ret; ret.value = value >> other; return ret; }
    template <typename U> Fix<s,T> operator <<(const U other) const { Fix<s,T> ret; ret.value = value << other; return ret; }

    Fix<s,T> operator +(const Fix<s,T>& other) const { Fix<s,T> ret; ret.value = value + other.value; return ret; }
    template <typename U> Fix<s,T> operator +(const U other) const { Fix<s,T> ret; ret.value = value + (other<<s); return ret; }

    Fix<s,T> operator -(const Fix<s,T>& other) const { Fix<s,T> ret; ret.value = value - other.value; return ret; }
    template <typename U> Fix<s,T> operator -(const U other) const { Fix<s,T> ret; ret.value = value - (other<<s); return ret; }

    template <unsigned int s2, typename T2> Fix<s,T> operator *(const Fix<s2,T2>& other) const { Fix<s,T> ret; ret.value = (value * other.value) >> s2; return ret; }
    template <typename U> Fix<s,T> operator *(const U other) const { Fix<s,T> ret; ret.value = value * other; return ret; }

    Fix<s,T> operator /(const Fix<s,T>& other) const { Fix<s,T> ret; ret.value = divide_real((value << s), other.value); return ret; }
    template <typename U> Fix<s,T> operator /(const U other) const { Fix<s,T> ret; ret.value = divide_real(value, other); return ret; }

    Fix<s,T>& operator +=(const Fix<s,T>& other) { value += other.value; return *this; }
    Fix<s,T>& operator -=(const Fix<s,T>& other) { value -= other.value; return *this; }
    Fix<s,T>& operator *=(const float other) { value *= other; return *this; }
    Fix<s,T>& operator *=(const int other) { value *= other; return *this; }
    Fix<s,T>& operator *=(const Fix<s,T>& other) { value = (value * other.value) >> s; return *this; }
    Fix<s,T>& operator /=(const float other) { value = divide_real(value, other); return *this; }
    Fix<s,T>& operator /=(const int other) { value = divide_real(value, other); return *this; }
    Fix<s,T>& operator /=(const Fix<s,T>& other) { value = divide_real((value << s), other.value); return *this; }

    constexpr bool operator >(const Fix<s,T>& other) const { return value > other.value; }
    constexpr bool operator <(const Fix<s,T>& other) const { return value < other.value; }
    template <typename U> constexpr bool operator >=(const U other) const { return value >= other<<s; }
    constexpr bool operator >=(const Fix<s,T>& other) const { return value >= other.value; }
    constexpr bool operator <=(const Fix<s,T>& other) const { return value <= other.value; }
    constexpr bool operator ==(const Fix<s,T>& other) const { return value == other.value; }
    constexpr bool operator !=(const Fix<s,T>& other) const { return value != other.value; }

    template <typename U>
    constexpr U toInteger() const
    {
        return value >> s;
    }

    constexpr float toFloat() const
    {
        return divide_real(static_cast<float>(value), static_cast<float>(1<<s));
    }

    template <typename U>
    void fromInteger(const U v)
    {
        value = v << s;
    }

    void fromFloat(const float v)
    {
        value = static_cast<T>(v * static_cast<float>(1<<s));
    }

    Fix<s,T>& normaliseAngle()
    {
        while(*this < Fix<s,T>(0))
            *this += Fix<s,T>(360);

        while(*this >= 360)
            *this -= Fix<s,T>(360);

        return *this;
    }

    static Fix<s,T> minStep()
    {
        Fix<s,T> ret;
        ret.value = 1;
        return ret;
    }

    static Fix<s,T> minValue()
    {
        Fix<s,T> ret;
        ret.value = std::numeric_limits<T>::min();
        return ret;
    }

    static Fix<s,T> maxValue()
    {
        Fix<s,T> ret;
        ret.value = std::numeric_limits<T>::max();
        return ret;
    }

    void print() const
    {
    }

    using type = T;
    constexpr static unsigned int precision = s;

    T value;
};

#endif // FIX_H
