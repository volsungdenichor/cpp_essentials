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

} /* namespace cpp_essentials::chrono */

#endif /* CPP_ESSENTIALS_CHRONO_TIME_HPP_ */