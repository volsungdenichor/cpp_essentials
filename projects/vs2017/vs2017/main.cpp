#include <iostream>
#include <string>
#include <millrind/core/type_traits.hpp>
#include <millrind/core/iterator_range.hpp>
#include <millrind/core/algorithm.hpp>
#include <millrind/core/opt.hpp>


using namespace millrind::core;

opt<double> square_root(double x)
{
    if (x >= 0.0)
        return some(std::sqrt(x));
    else
        return none;
}

void run()
{
    opt<double> a = some(5.0);
    opt<double> b = some(10.0);
    opt<double> c = some(20.0);

    std::cout << (a & b & c) << std::endl;
    std::cout << (a | b | c) << std::endl;
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