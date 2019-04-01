#include <iostream>
#include <vector>

#include <cpp_essentials/core/arithmetic_functors.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/core/serialization.hpp>
#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/graphs/algorithm.hpp>
#include <cpp_essentials/graphs/tree.hpp>

#include <cpp_essentials/sq/sq.hpp>

using namespace cpp_essentials;



void run()
{
    graphs::binary_tree<int> graph;
    auto root = graph.root(3);
    auto f = graph.set_left_child(root, 5);
    auto s = graph.set_right_child(root, 7);
    graph.set_left_child(f, 13);
    graph.set_right_child(f, 15);
    graph.set_left_child(s, 21);

    for (const auto& item : graph.root().postorder() | sq::iterate())
    {
        std::cout << std::string(item.depth() * 4, ' ') << *item << std::endl;
    }
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
