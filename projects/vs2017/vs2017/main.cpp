#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/arithmetic_functors.hpp>
#include <cpp_essentials/core/math_functors.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/output.hpp>
#include <cpp_essentials/core/tuple.hpp>

#include <cpp_essentials/sq/numeric.hpp>
#include <cpp_essentials/sq/map.hpp>
#include <cpp_essentials/sq/zip.hpp>

using namespace cpp_essentials;

void run()
{
    std::vector<std::vector<int>> vect = {
        { 1, 2, 5 },
        { 2, 6, 1 },
        { 0, 1, 2, 3, 4 },
        {},
    };

    vect
        | sq::map(core::minmax_element.as<core::return_optional>())
        | core::for_each([](auto r) { std::cout << r << std::endl; });
}

int main()
{
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception: "<< ex.what() << std::endl;
    }
    return 0;
}