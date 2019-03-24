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
#include <cpp_essentials/core/format.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/core/channel.hpp>

using namespace cpp_essentials;

template <class Iter, class Func>
class successors_iterator
    : public core::iterator_facade
    < successors_iterator<Iter, Func>
    , std::forward_iterator_tag
    , std::invoke_result_t<Func, cc::iter_ref<Iter>, cc::iter_ref<Iter>>
    , std::ptrdiff_t>
{
public:
    using base_type = core::iterator_facade
        < successors_iterator<Iter, Func>
        , std::forward_iterator_tag
        , std::invoke_result_t<Func, cc::iter_ref<Iter>, cc::iter_ref<Iter>>
        , std::ptrdiff_t>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

        successors_iterator() = default;

    successors_iterator(Iter iter, Func func)
        : _func{ std::move(func) }
        , _iter{ iter }
        , _prev{ iter }
    {
    }

    reference ref() const
    {
        return _func(*_iter, *_prev);
    }

    void inc()
    {
        _prev = _iter;
        ++_iter;
    }

    bool is_equal(const successors_iterator& other) const
    {
        return _iter == other._iter;
    }

private:
    Iter _iter;
    Iter _prev;
    core::detail::default_constructible_func<Func> _func;
};

struct successors_fn
{
    template <class Range, class Func>
    auto operator ()(Range&& range, Func func) const
    {
        return core::make_range(
            successors_iterator{ std::begin(range), func },
            successors_iterator{ std::end(range), func });
    }
};

static constexpr auto successors = core::adaptable{ successors_fn{} };

struct message
{
    std::string text;

    message(std::string text)
        : text{ std::move(text) }
    {
    }
};

void run()
{
    core::iochannel<std::unique_ptr<message>> channel;

    auto send = [](core::ochannel<std::unique_ptr<message>>& o)
    {
        for (int i = 0; i < 10; ++i)
        {
            o.push(std::make_unique<message>(std::to_string(i * 10)));
            std::this_thread::sleep_for(std::chrono::seconds{ 3 });
        }
        o.push(nullptr);
    };

    auto receive = [](core::ichannel<std::unique_ptr<message>>& o)
    {
        while (true)
        {
            std::unique_ptr<message> value;
            if (o.try_pop(value))
            {
                if (v)
                std::cout << value->text << std::endl;
                if (value == nullptr)
                {
                    std::cout << "Bye!" << std::endl;
                    return;
                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::seconds{ 1 });
            }
        }
    };

    auto sender = std::thread{ send, std::ref(channel) };
    auto receiver = std::thread{ receive, std::ref(channel) };
    sender.join();
    receiver.join();

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
