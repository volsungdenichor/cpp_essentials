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
#include <cpp_essentials/gx/morphological_operations.hpp>
#include <cpp_essentials/gx/kernels.hpp>
#include <cpp_essentials/gx/lookup_table.hpp>
#include <cpp_essentials/gx/drawing_context.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <complex>
#include <cpp_essentials/geo/intersection.hpp>
#include <cpp_essentials/geo/projection.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/coordinates_conversion.hpp>
#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/core/expected.hpp>

using namespace cpp_essentials;

template <class T>
std::tuple<T, T> split(T x)
{
    T i;
    T f = std::modf(x, &i);
    return { i, f };
}

template <class T>
std::tuple<T, T> split_(T x)
{
    T f = x - std::floor(x);
    T i = std::floor(x);
    return { i, f };
}


void run()
{
    gx::rgb_image img{ { 200, 200 } };    
    auto dc = gx::make_drawing_context(img);
    dc.draw_line_smooth({ 10, 10 }, { 50, 50 }, gx::colors::rosy_brown);
    img | sq::map(gx::brightness(100)) | sq::overwrite(img);
    char ch = ' ';
}

int main()
{
    try
    {
        run();
    }
    catch (std::exception& ex)
    {
        std::exception_ptr o;
        std::cerr << "exception: " << ex.what() << std::endl;
    }
    return 0;
}
