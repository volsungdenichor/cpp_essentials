#include <iostream>
#include <vector>
#include <cpp_essentials/graphs/tree.hpp>
#include <cpp_essentials/sq/sq.hpp>

template <class T, class... Children>
cpp_essentials::graphs::tree<T> node(T value, Children... children)
{
    cpp_essentials::graphs::tree<T> result;
    auto it = result.root(std::move(value));

    const auto insert = [&](auto& child)
    {
        result.append_child(it, child.root());
    };

    (..., insert(children));

    return result;
}

void run()
{
    using namespace std::string_literals;
    using namespace cpp_essentials;

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

