#ifndef CPP_ESSENTIALS_CHRONO_TIME_HPP_
#define CPP_ESSENTIALS_CHRONO_TIME_HPP_

#pragma once

#include <iostream>
#include <chrono>

namespace cpp_essentials::chrono
{

struct duration
{
public:
    using value_type = double;

    duration() = default;

    explicit duration(value_type value)
        : _value{ value }
    {
    }

    friend duration operator +(duration item)
    {
        return duration{ item._value };
    }

    friend duration operator -(duration item)
    {
        return duration{ -item._value };
    }

    friend duration& operator +=(duration& item, duration other)
    {
        item._value += other._value;
        return item;
    }

    friend duration operator +(duration lhs, duration rhs)
    {
        return lhs += rhs;
    }

    friend duration& operator -=(duration& item, duration other)
    {
        item._value -= other._value;
        return item;
    }

    friend duration operator -(duration lhs, duration rhs)
    {
        return lhs -= rhs;
    }

    friend duration& operator *=(duration& item, value_type other)
    {
        item._value *= other;
        return item;
    }

    friend duration operator *(duration lhs, value_type rhs)
    {
        return lhs *= rhs;
    }

    friend duration operator *(value_type lhs, duration rhs)
    {
        return rhs * lhs;
    }

    friend value_type operator /(duration lhs, duration rhs)
    {
        return lhs._value / rhs._value;
    }

    friend duration& operator /=(duration& item, value_type other)
    {
        item._value /= other;
        return item;
    }

    friend duration operator /(duration lhs, value_type rhs)
    {
        return lhs /= rhs;
    }

    friend bool operator ==(duration lhs, duration rhs)
    {
        return lhs._value == rhs._value;
    }

    friend bool operator !=(duration lhs, duration rhs)
    {
        return lhs._value != rhs._value;
    }

    friend bool operator <(duration lhs, duration rhs)
    {
        return lhs._value < rhs._value;
    }

    friend bool operator >(duration lhs, duration rhs)
    {
        return lhs._value > rhs._value;
    }

    friend bool operator <=(duration lhs, duration rhs)
    {
        return lhs._value <= rhs._value;
    }

    friend bool operator >=(duration lhs, duration rhs)
    {
        return lhs._value >= rhs._value;
    }

    friend std::ostream& operator <<(std::ostream& os, duration item)
    {
        return os << item._value;
    }

    value_type _value;
};

struct time_point
{
public:
    using value_type = double;

    time_point() = default;

    explicit time_point(value_type value)
        : _value{ value }
    {
    }

    friend time_point& operator +=(time_point& item, duration other)
    {
        item._value += other._value;
        return item;
    }

    friend time_point operator +(time_point lhs, duration rhs)
    {
        return lhs += rhs;
    }

    friend time_point operator +(duration lhs, time_point rhs)
    {
        return rhs + lhs;
    }

    friend time_point& operator -=(time_point& item, duration other)
    {
        item._value -= other._value;
        return item;
    }

    friend time_point operator -(time_point lhs, duration rhs)
    {
        return lhs -= rhs;
    }

    friend duration operator -(time_point lhs, time_point rhs)
    {
        return duration{ lhs._value - rhs._value };
    }

    friend time_point& operator ++(time_point& item)
    {
        return item += duration{ 1.0 };
    }

    friend time_point operator ++(time_point& item, int)
    {
        time_point temp{ item };
        ++temp;
        return temp;
    }

    friend time_point& operator --(time_point& item)
    {
        return item -= duration{ 1.0 };
    }

    friend time_point operator --(time_point& item, int)
    {
        time_point temp{ item };
        --temp;
        return temp;
    }

    friend bool operator ==(time_point lhs, time_point rhs)
    {
        return lhs._value == rhs._value;
    }

    friend bool operator !=(time_point lhs, time_point rhs)
    {
        return lhs._value != rhs._value;
    }

    friend bool operator <(time_point lhs, time_point rhs)
    {
        return lhs._value < rhs._value;
    }

    friend bool operator >(time_point lhs, time_point rhs)
    {
        return lhs._value > rhs._value;
    }

    friend bool operator <=(time_point lhs, time_point rhs)
    {
        return lhs._value <= rhs._value;
    }

    friend bool operator >=(time_point lhs, time_point rhs)
    {
        return lhs._value >= rhs._value;
    }

    friend std::ostream& operator <<(std::ostream& os, time_point item)
    {
        return os << item._value;
    }

    value_type _value;
};

inline const duration days{ 1.0 };
inline const duration hours = days / 24.0;
inline const duration minutes = hours / 60.0;
inline const duration seconds = minutes / 60.0;
inline const duration milliseconds = seconds / 1000.0;
inline const duration weeks = days * 7.0;


inline time_point trunc(time_point tp)
{
    return time_point{ std::floor(tp._value - 0.5) + 0.5 };
}

inline std::pair<time_point, duration> split(time_point tp)
{
    auto d = trunc(tp);
    return std::make_pair(d, tp - d);
}

inline time_point from_unix_time(std::uint64_t unix_time_ms)
{
    static const time_point epoch{ 2440587.5 };
    return epoch + (double)unix_time_ms * milliseconds;
}

inline time_point from_time_point(std::chrono::system_clock::time_point tp)
{
    const auto unix_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    return from_unix_time(unix_time_ms);
}

inline time_point now()
{
    return from_time_point(std::chrono::system_clock::now());
}

} /* namespace cpp_essentials::chrono */

#endif /* CPP_ESSENTIALS_CHRONO_TIME_HPP_ */