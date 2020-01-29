#ifndef CPP_ESSENTIALS_TERM_TERM_HPP_
#define CPP_ESSENTIALS_TERM_TERM_HPP_

#pragma once

#include <ostream>

namespace cpp_essentials::term
{

struct ansi
{
    const int value;

    friend std::ostream& operator <<(std::ostream& os, const ansi& item)
    {
        return os << "\33[" << item.value << "m";
    }
};

enum class color
{
    black = 0,
    dark_red = 1,
    dark_green = 2,
    dark_yellow = 3,
    dark_blue = 4,
    dark_magenta = 5,
    dark_cyan = 6,
    gray = 7,
    dark_gray = 60,
    red = 61,
    green = 62,
    yellow = 63,
    blue = 64,
    magenta = 65,
    cyan = 66,
    white = 67,
};

constexpr inline ansi fg(color c)
{
    return ansi{ 30 + (int)c };
}

constexpr inline ansi bg(color c)
{
    return ansi{ 40 + (int)c };
}

static constexpr inline auto fg_black = fg(color::black);
static constexpr inline auto fg_dark_red = fg(color::dark_red);
static constexpr inline auto fg_dark_green = fg(color::dark_green);
static constexpr inline auto fg_dark_yellow = fg(color::dark_yellow);
static constexpr inline auto fg_dark_blue = fg(color::dark_blue);
static constexpr inline auto fg_dark_magenta = fg(color::dark_magenta);
static constexpr inline auto fg_dark_cyan = fg(color::dark_cyan);
static constexpr inline auto fg_gray = fg(color::gray);
static constexpr inline auto fg_dark_gray = fg(color::dark_gray);
static constexpr inline auto fg_red = fg(color::red);
static constexpr inline auto fg_green = fg(color::green);
static constexpr inline auto fg_yellow = fg(color::yellow);
static constexpr inline auto fg_blue = fg(color::blue);
static constexpr inline auto fg_magenta = fg(color::magenta);
static constexpr inline auto fg_cyan = fg(color::cyan);
static constexpr inline auto fg_white = fg(color::white);

static constexpr inline auto bg_black = bg(color::black);
static constexpr inline auto bg_dark_red = bg(color::dark_red);
static constexpr inline auto bg_dark_green = bg(color::dark_green);
static constexpr inline auto bg_dark_yellow = bg(color::dark_yellow);
static constexpr inline auto bg_dark_blue = bg(color::dark_blue);
static constexpr inline auto bg_dark_magenta = bg(color::dark_magenta);
static constexpr inline auto bg_dark_cyan = bg(color::dark_cyan);
static constexpr inline auto bg_gray = bg(color::gray);
static constexpr inline auto bg_dark_gray = bg(color::dark_gray);
static constexpr inline auto bg_red = bg(color::red);
static constexpr inline auto bg_green = bg(color::green);
static constexpr inline auto bg_yellow = bg(color::yellow);
static constexpr inline auto bg_blue = bg(color::blue);
static constexpr inline auto bg_magenta = bg(color::magenta);
static constexpr inline auto bg_cyan = bg(color::cyan);
static constexpr inline auto bg_white = bg(color::white);

static constexpr inline auto black = fg_black;
static constexpr inline auto dark_red = fg_dark_red;
static constexpr inline auto dark_green = fg_dark_green;
static constexpr inline auto dark_yellow = fg_dark_yellow;
static constexpr inline auto dark_blue = fg_dark_blue;
static constexpr inline auto dark_magenta = fg_dark_magenta;
static constexpr inline auto dark_cyan = fg_dark_cyan;
static constexpr inline auto gray = fg_gray;
static constexpr inline auto dark_gray = fg_dark_gray;
static constexpr inline auto red = fg_red;
static constexpr inline auto green = fg_green;
static constexpr inline auto yellow = fg_yellow;
static constexpr inline auto blue = fg_blue;
static constexpr inline auto magenta = fg_magenta;
static constexpr inline auto cyan = fg_cyan;
static constexpr inline auto white = fg_white;

static constexpr inline auto reset = ansi{ 0 };

} /* namespace cpp_essentials::term */

#endif /* CPP_ESSENTIALS_TERM_TERM_HPP_ */