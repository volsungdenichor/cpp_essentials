#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/cc/cc.hpp>

#include <cpp_essentials/math/arithmetic_functors.hpp>
#include <cpp_essentials/math/functors.hpp>

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/views.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/output.hpp>
#include <cpp_essentials/core/tuple.hpp>

#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/format.hpp>

#include <cpp_essentials/geo/matrix.hpp>
#include <cpp_essentials/geo/interval.hpp>
#include <cpp_essentials/geo/spatial_relations.hpp>

using namespace cpp_essentials;

void run()
{
    geo::interval<int> v = { 2, 5 };
    geo::interval<int> a = { 3, 4 };
    std::cout << geo::interpolate(v, 0.5) << std::endl;
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
