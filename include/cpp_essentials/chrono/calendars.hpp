#ifndef CPP_ESSENTIALS_CHRONO_CALENDARS_HPP_
#define CPP_ESSENTIALS_CHRONO_CALENDARS_HPP_

#pragma once

#include <iomanip>
#include <cmath>
#include <chrono>

#include <cpp_essentials/chrono/time.hpp>

namespace cpp_essentials::chrono
{

inline std::pair<double, double> div(double a, double b)
{
    return { std::floor(a / b), std::fmod(a, b) };
}

inline time_point from_time_point(std::chrono::system_clock::time_point tp)
{
    static const double epoch = 2440587.5;
    const auto seconds_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
    return time_point{ epoch + (seconds_since_epoch / (60 * 60 * 24)) };
}

inline time_point now()
{
    return from_time_point(std::chrono::system_clock::now());
}


struct unix_time
{
    static const inline double epoch = 2440587.5;
    using date = std::uint64_t;

    static time_point to(date date)
    {
        return time_point{ epoch + (date / (60 * 60 * 24)) };
    }

    static date from(time_point tp)
    {
        return (date)std::round(((tp._value - epoch) * (60 * 60 * 24 * 1000)) / 1000);
    }

    static date now()
    {
        return from(chrono::now());
    }
};

struct gregorian
{
    static const inline double epoch = 1721425.5;

    struct date
    {
        int year;
        char month;
        char day;

        friend std::ostream& operator <<(std::ostream& os, const date& item)
        {
            return os
                << item.year
                << "." << std::setfill('0') << std::setw(2) << (int)item.month
                << "." << std::setfill('0') << std::setw(2) << (int)item.day;
        }
    };

    static bool is_leap(int year)
    {
        return ((year % 4) == 0)
            && (!(((year % 100) == 0) && ((year % 400) != 0)));
    }

    static time_point to(const date& date)
    {
        auto[year, month, day] = date;
        return time_point{ epoch - 1
            + 365 * (year - 1)
            + std::floor((year - 1) / 4)
            + -std::floor((year - 1) / 100)
            + std::floor((year - 1) / 400)
            + std::floor((((367 * month) - 362) / 12)
            + (month <= 2 ? 0 : is_leap(year) ? -1 : -2)
            + day) };
    }

    static date from(time_point tp)
    {
        const auto wjd = std::floor(tp._value - 0.5) + 0.5;
        const auto depoch = wjd - epoch;
        const auto[quadricent, dqc] = div(depoch, 146097.0);
        const auto[cent, dcent] = div(dqc, 36524.0);
        const auto[quad, dquad] = div(dcent, 1461.0);
        const auto yindex = std::floor(dquad / 365);
        auto year = (int)((quadricent * 400) + (cent * 100) + (quad * 4) + yindex);
        if (cent != 4 && yindex != 4)
        {
            year++;
        }
        const auto yearday = wjd - to({ year, 1, 1 })._value;
        const auto leap_adj = wjd >= to({ year, 3, 1 })._value
            ? is_leap(year) ? 1 : 2
            : 0;
        const auto month = (char)std::floor((((yearday + leap_adj) * 12) + 373) / 367);
        const auto day = (char)((wjd - to({ year, month, 1 })._value) + 1);

        return { year, month, day };
    }

    static date now()
    {
        return from(chrono::now());
    }
};

struct iso
{
    struct date
    {
        int year;
        char week;
        char day;

        friend std::ostream& operator <<(std::ostream& os, const date& item)
        {
            return os
                << item.year
                << "-W" << std::setfill('0') << std::setw(2) << (int)item.week
                << "-" << (int)item.day;
        }
    };

    static time_point to(const date& date)
    {
        const auto[year, week, day] = date;
        return time_point{ (double)(day + n_weeks(0, gregorian::to({ year - 1, 12, 28 }), week)) };
    }

    static date from(time_point tp)
    {
        int year = gregorian::from(time_point{ tp._value - 3.0 }).year;
        if (tp >= to({ year + 1, 1, 1 }))
        {
            ++year;
        }
        const char week = (char)(std::floor((tp._value - to({ year, 1, 1 })._value) / 7) + 1);
        const auto day = get_weekday(tp);
        return { year, week, day };
    }

    static date now()
    {
        return from(chrono::now());
    }

    static char get_weekday(time_point tp)
    {
        auto res = (char)std::fmod(std::floor((tp._value + 1.5)), 7.0);
        return (res + 7) % 7;
    }

private:
    static int n_weeks(int weekday, time_point jd, int nthweek)
    {
        auto j = 7 * nthweek;

        if (nthweek > 0)
        {
            j += previous_weekday(weekday, jd);
        }
        else
        {
            j += next_weekday(weekday, jd);

        }
        return j;
    }

    static int previous_weekday(int weekday, time_point jd)
    {
        return search_weekday(weekday, jd, -1, duration{ 1 });
    }

    static int previous_or_current_weekday(int weekday, time_point jd)
    {
        return search_weekday(weekday, jd, +1, duration{ 0 });
    }

    static int next_weekday(int weekday, time_point jd)
    {
        return search_weekday(weekday, jd, +1, duration{ 7 });
    }

    static int search_weekday(int weekday, time_point jd, int direction, duration offset)
    {
        return weekday_before(weekday, jd + (direction * offset));
    }

    static int weekday_before(int weekday, time_point jd)
    {
        auto res = jd - duration{ (double)(get_weekday(jd - duration{ (double)weekday })) };
        return (int)res._value;
    }
};

struct iso_day
{
    struct date
    {
        int year;
        short day;

        friend std::ostream& operator <<(std::ostream& os, const date& item)
        {
            return os
                << item.year
                << "-" << std::setfill('0') << std::setw(3) << item.day;
        }
    };

    static time_point to(const date& date)
    {
        const auto[year, day] = date;
        return gregorian::to({ year, 1, 1 }) + duration{ day - 1.0 };
    }

    static date from(time_point tp)
    {
        const auto year = gregorian::from(tp).year;
        const auto day = (short)(std::floor(tp._value - gregorian::to({ year, 1, 1 })._value) + 1);
        return { year, day };
    }

    static date now()
    {
        return from(chrono::now());
    }
};

struct julian
{
    struct date
    {
        int year;
        char month;
        char day;

        friend std::ostream& operator <<(std::ostream& os, const date& item)
        {
            return os << item.year << "." << (int)item.month << "." << (int)item.day;
        }
    };

    static bool is_leap(int year)
    {
        return year % 4 == (year > 0) ? 0 : 3;
    }

    static time_point to(const date& date)
    {
        auto[year, month, day] = date;
        if (year < 1)
        {
            ++year;
        }

        if (month <= 2)
        {
            --year;
            month += 12;
        }

        return time_point{ std::floor((365.25 * (year + 4716))) + std::floor((30.6001 * (month + 1))) + day - 1524.5 };
    }

    static date from(time_point tp)
    {
        const auto td = tp._value + 0.5;
        const auto z = std::floor(td);

        const auto a = z;
        const auto b = a + 1524.0;
        const auto c = std::floor((b - 122.1) / 365.25);
        const auto d = std::floor(365.25 * c);
        const auto e = std::floor((b - d) / 30.6001);

        const auto month = (char)(std::floor((e < 14) ? (e - 1) : (e - 13)));
        const auto day = (char)(b - d - std::floor(30.6001 * e));

        auto year = (int)(std::floor((month > 2) ? (c - 4716) : (c - 4715)));
        if (year < 1)
        {
            --year;
        }

        return { year, month, day };
    }

    static date now()
    {
        return from(chrono::now());
    }
};

} /* namespace cpp_essentials::chrono */

#endif /* CPP_ESSENTIALS_CHRONO_CALENDARS_HPP_ */