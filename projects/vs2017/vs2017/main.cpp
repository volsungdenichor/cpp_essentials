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
#include <cpp_essentials/sq/filter.hpp>
#include <cpp_essentials/sq/slice.hpp>
#include <cpp_essentials/sq/zip.hpp>
#include <cpp_essentials/sq/reverse.hpp>
#include <cpp_essentials/sq/containers.hpp>
#include <cpp_essentials/sq/index.hpp>

using namespace cpp_essentials;

void run()
{
    sq::range(10)
        // | sq::to_vector()
        | sq::reverse()
        | sq::take(3)
        | sq::index()
        | core::for_each([](auto&& x) { std::cout << x << std::endl; })
        ;
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