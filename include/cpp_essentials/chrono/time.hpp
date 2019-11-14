#ifndef CPP_ESSENTIALS_CHRONO_TIME_HPP_
#define CPP_ESSENTIALS_CHRONO_TIME_HPP_

#pragma once

#include <iostream>

namespace cpp_essentials::chrono
{

struct duration
{
public:
    duration() = default;

    explicit duration(double value)
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

    friend duration& operator *=(duration& item, double other)
    {
        item._value *= other;
        return item;
    }

    friend duration operator *(duration lhs, double rhs)
    {
        return lhs *= rhs;
    }

    friend duration operator *(double lhs, duration rhs)
    {
        return rhs * lhs;
    }

    friend double operator /(duration lhs, duration rhs)
    {
        return lhs._value / rhs._value;
    }

    friend duration& operator /=(duration& item, double other)
    {
        item._value /= other;
        return item;
    }

    friend duration operator /(duration lhs, double rhs)
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

    double _value;
};

struct time_point
{
public:
    time_point() = default;

    explicit time_point(double value)
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

    double _value;
};

inline const duration days{ 1.0 };
inline const duration hours = days / 24.0;
inline const duration minutes = hours / 60.0;
inline const duration seconds = minutes / 60.0;
inline const duration weeks = days * 7.0;


inline time_point trunc(time_point tp)
{
    return time_point{ std::floor(tp._value + 0.5) - 0.5 };
}

inline std::pair<time_point, duration> split(time_point tp)
{
    auto d = trunc(tp);
    return std::make_pair(d, tp - d);
}

} /* namespace cpp_essentials::chrono */

#endif /* CPP_ESSENTIALS_CHRONO_TIME_HPP_ */