#include <iostream>
#include <string>
#include <millrind/core/type_traits.hpp>
#include <millrind/core/iterator_range.hpp>
#include <millrind/core/algorithm.hpp>
#include <millrind/core/opt.hpp>


#define LAMBDA(...) [](auto&& _) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }

using namespace millrind::core;

static constexpr auto sqr = [](auto x) { return x * x; };

opt<double> square_root(double x)
{
    if (x >= 0.0)
        return some(std::sqrt(x));
    else
        return none;
}

void run()
{
    double x = 11;
    opt<double&> value = some(std::ref(x));

    auto res = value
        .filter_map(square_root)
        .map(sqr)
        .filter(LAMBDA(_ > 100))
        .or_else([] { return some(99.0); })
        .value_or_throw([] { return "Dupa"; });

    std::cout << res << std::endl;
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