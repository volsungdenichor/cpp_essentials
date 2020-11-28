#include <iostream>
#include <string>
#include <millrind/core/type_traits.hpp>
#include <millrind/core/iterator_range.hpp>
#include <millrind/core/algorithm.hpp>
#include <millrind/core/opt.hpp>

template <class T, std::size_t Index, std::size_t Last>
struct tuple_printer
{
    static void print(std::ostream& os, const T& value, const char* separator)
    {
        os << std::get<Index>(value) << separator;
        tuple_printer<T, Index + 1, Last>::print(os, value, separator);
    }
};

template <class T, std::size_t Index>
struct tuple_printer<T, Index, Index>
{
    static void print(std::ostream& os, const T& value, const char* separator)
    {
        os << std::get<Index>(value);
    }
};

namespace std
{

template <class... Args>
ostream& operator <<(ostream& os, const tuple<Args...>& value)
{
    os << "(";

    tuple_printer<tuple<Args...>, 0, sizeof...(Args) - 1>::print(os, value, ";");

    os << ")";

    return os;
}

template <class T1, class T2>
ostream& operator <<(ostream& os, const pair<T1, T2>& value)
{
    os << "(";

    tuple_printer<pair<T1, T2>, 0, 1>::print(os, value, ";");

    os << ")";

    return os;
}

}

struct tie_fn
{
    template <class... Args>
    auto operator()(Args&&... args) const
    {
        return std::tuple{ FORWARD(args)... };
    }
};

static constexpr auto to_tuple = tie_fn{};


using namespace millrind::core;

opt<double> square_root(double x)
{
    if (x >= 0.0)
        return some(std::sqrt(x));
    else
        return none;
}

void show(iterable<double> range)
{
    range.for_each(LAMBDA(std::cout << _ << std::endl));
}

void run()
{
    std::vector<int> a = { 0, 2, 4, 8, 9 };
    std::vector<int> b = { 9, 7, 5, 1, 0 };

    int cur = 1;

    zip(a, b)
        .for_each(LAMBDA(std::cout << _ << std::endl));

}

int main()
{
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: " << ex.what() << std::endl;
    }
}