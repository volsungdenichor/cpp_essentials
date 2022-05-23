#include <iostream>
#include <vector>
#include <cpp_essentials/graphs/tree.hpp>
#include <cpp_essentials/sq/sq.hpp>


void run()
{
    using namespace std::string_literals;
    using namespace cpp_essentials;

    using graphs::node;

    graphs::tree<std::string> t = node("Europe"s,
        node("France"s),
        node("Poland"s,
            node("Dolnoslaskie"s,
                node("Wroclaw"s),
                node("Legnica"s),
                node("Jelenia Gora"s))),
        node("Germany"s,
            node("Bayern"s,
                node("Munchen"s))));

    for (auto it : core::views::iterate(t))
    {
        std::cout << std::string(it.depth() * 4, ' ') << *it << std::endl;
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

