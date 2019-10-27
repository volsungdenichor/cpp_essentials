#pragma once

#include <map>
#include <cpp_essentials/core/assertions.hpp>

namespace cpp_essentials::math
{

namespace detail
{

template <class T>
struct binomial_fn
{
    using value_type = T;

    value_type operator ()(int n, int k) const
    {
        return cache::instance()(n, k);
    }

private:
    struct cache
    {
        using pair_type = std::pair<int, int>;
        using cache_type = std::map<pair_type, value_type>;

        cache(const cache&) = delete;
        cache(cache&&) = delete;

        cache& operator =(const cache&) = delete;
        cache& operator =(cache&&) = delete;

        static const cache& instance()
        {
            static const cache inst = {};
            return inst;
        }

        value_type operator ()(int n, int k) const
        {
            EXPECTS(k <= n);

            const auto p = pair_type{ n, k };

            if (auto it = _cache.find(p); it != _cache.end())
            {
                return it->second;
            }

            const auto result = k == 0 || k == n
                ? 1
                : (*this)(n - 1, k - 1) + (*this)(n - 1, k);

            _cache[p] = result;

            return result;
        }

        mutable cache_type _cache;
    };
};

} /* namespace detail */

static const detail::binomial_fn<long long> binomial = {};

} /* namespace cpp_essentials::math */
