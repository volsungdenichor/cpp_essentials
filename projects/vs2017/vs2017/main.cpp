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
#include <complex>
#include <cpp_essentials/geo/intersection.hpp>
#include <cpp_essentials/geo/projection.hpp>
#include <cpp_essentials/geo/distance.hpp>
#include <cpp_essentials/geo/circle.hpp>
#include <cpp_essentials/geo/coordinates_conversion.hpp>

using namespace cpp_essentials;

template <class T, class Func>
class successors_iterator
    : public core::iterator_facade
    < successors_iterator<T, Func>
    , std::forward_iterator_tag
    , const T&
    , std::ptrdiff_t>
{
public:
    using base_type = core::iterator_facade
        < successors_iterator<T, Func>
        , std::forward_iterator_tag
        , const T&
        , std::ptrdiff_t>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    successors_iterator() = default;

    successors_iterator(T current, Func func, int index = std::numeric_limits<int>::min())
        : _current{ std::move(current) }
        , _func{ std::move(func) }
        , _index{ index }
    {
    }

    reference ref() const
    {
        return _current;
    }

    void inc()
    {
        ++_index;
        _current = _func(_current);
    }

    bool is_equal(const successors_iterator& other) const
    {
        return _index == other._index;
    }

private:
    T _current;
    core::detail::default_constructible_func<Func> _func;
    int _index;
};

struct successors_fn
{
    template <class T, class Func>
    auto operator ()(T init, Func func) const
    {
        return core::make_range(
            successors_iterator{ init, func, 0 },
            successors_iterator{ init, func });
    }
};

static constexpr auto successors = successors_fn{};


void run()
{
    successors(1, core::multiplies(2))
        | sq::take(10)
        | sq::write(std::cout, " ");

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
