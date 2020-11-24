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

void show(iterable<double> range)
{
    range.for_each(LAMBDA(std::cout << _ << std::endl));
}

void run()
{
    std::vector<int> vect = { 1,2,3,4,5,6,7,8 };
    int index = 0;
    show(make_generator([&vect, index]() mutable -> opt<int&>
        {
            if (index >= vect.size())
                return none;
            
            int& res = vect[index];
            index += 2;
            return some(&res);
        })
        .filter_map(square_root));
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