#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/geo/bounding_box.hpp>
#include <cpp_essentials/core/string_views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/core/string_functors.hpp>
#include <string_view>
#include <cpp_essentials/meta/serialization_json.hpp>
#include <cpp_essentials/gx/bitmap.hpp>
#include <complex>
#include <cpp_essentials/geo/intersection.hpp>
#include <cpp_essentials/geo/projection.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/coordinates_conversion.hpp>

#include <cpp_essentials/rs/rs.hpp>

using namespace cpp_essentials;

void run()
{
    std::vector<std::vector<int>> vect = {
        { 1, 2, 3 },
        { 10, 20, 30 },
    };

    auto f = [](int x) -> std::optional<std::string>
    {
        return core::eval_optional(x % 5 == 0, [=]() { return core::str("[", x, "]"); });
    };

    auto [a, b] = rs::iter(vect)
        | rs::flatten()
        | rs::partition([](auto x) { return  x % 3 == 0; })
        ;

    //std::cout << a << std::endl;
    rs::for_each(a, core::output(std::cout, " "));
    std::cout << std::endl;
    b | rs::for_each(core::output(std::cout, " "));
    std::cout << std::endl;

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
    return 0;
}
