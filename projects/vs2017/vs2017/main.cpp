#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>

#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/meta/serialization_json.hpp>

#include <cpp_essentials/math/functors.hpp>
#include <cpp_essentials/core/output.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/chunk.hpp>

#include <cpp_essentials/sq/sq.hpp>
#include <cpp_essentials/core/elementwise.hpp>

#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/math/polynomial.hpp>

#include <json.hpp>
#include <cpp_essentials/math/regression.hpp>
#include <cpp_essentials/arrays/array.hpp>
#include <cpp_essentials/arrays/transformations.hpp>
#include <cpp_essentials/gx/bitmap.hpp>
#include <cpp_essentials/gx/colors.hpp>
#include <cpp_essentials/gx/drawing_context.hpp>

using namespace cpp_essentials;

enum class Color
{
    red,
    green,
    blue,
};

struct Test
{
    int a;
    std::string b;
    Color c;
    int d;
};

template <>
inline auto meta::register_type<Test>()
{    
    return structure(
        "Test"
        , member(&Test::a, "a")
        , member(&Test::b, "b")
        , member(&Test::c, "c")
        , member(&Test::d, "d")
    );
}

template <>
inline auto meta::register_type<Color>()
{    
    return enumeration(
        "Color"
        , enum_value(Color::red, "red")
        , enum_value(Color::green, "green")
        , enum_value(Color::blue, "blue")
    );
}

void print(const nlohmann::json& json)
{
    std::cout << json.dump(2) << std::endl;
}

template <class T>
void print(const T& item)
{
    nlohmann::json json = item;
    print(json);
}

void print_text(core::cstring_view text)
{
    std::cout << "[" << text << "] (" << text.size() << ")" << std::endl;
}

void run()
{
    Test test;

    test.b = "Anna Maria";
    meta::get(test, "a") = 91;
    meta::get(test, "e") = 0;
    print(test);
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
