#pragma once

#include <string>
#include <string_view>
#include <type_traits>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <tuple>
#include <iterator>
#include <optional>

namespace md5
{

using size_type = unsigned int;

using u8 = unsigned char;
using u32 = std::uint32_t;

template <class T>
class span
{
public:
    using iterator = T*;

    constexpr span() noexcept
        : _begin{}
        , _end{}
    {
    }

    constexpr span(iterator begin, iterator end) noexcept
        : _begin{ begin }
        , _end{ end }
    {
    }

    constexpr span(iterator begin, size_type size) noexcept
        : span{ begin, begin + size }
    {
    }

    template <class U, std::size_t N>
    constexpr span(std::array<U, N>& array)
        : span{ array.data(), (size_type)array.size() }
    {
    }

    template <class U, std::size_t N>
    constexpr span(const std::array<U, N>& array)
        : span{ array.data(), (size_type)array.size() }
    {
    }

    template <class U>
    constexpr span(span<U> other) noexcept
        : span{ other.begin(), other.end() }
    {
    }

    template <class Type = T, class = std::enable_if_t<!std::is_void_v<T>>>
    constexpr const void* data() const noexcept
    {
        return (const void*)_begin;
    }

    template <class Type = T, class = std::enable_if_t<!std::is_void_v<T>>>
    constexpr void* data() noexcept
    {
        return (void*)_begin;
    }

    constexpr iterator begin() const noexcept
    {
        return _begin;
    }

    constexpr iterator end() const noexcept
    {
        return _end;
    }

    constexpr size_type size() const noexcept
    {
        return (size_type)(end() - begin());
    }

    constexpr size_type size_bytes() const noexcept
    {
        return size() * sizeof(T);
    }

    template <class Type = T, class = std::enable_if_t<!std::is_void_v<T>>>
    T& operator[](size_type index) const noexcept
    {
        return _begin[index];
    }    

    T* _begin;
    T* _end;
};

template <class T>
using const_span = span<const T>;

template <class T>
constexpr const_span<u8> as_bytes(span<T> span) noexcept
{
    return { (const u8*)span.data(), span.size_bytes() };
}

template <class T>
constexpr span<u8> as_writable_bytes(span<T> span) noexcept
{
    return { (u8*)span.data(), span.size_bytes() };
}

template <class T, class = void>
struct handler;

class generator
{
public:
    static const inline auto s_11 = 7;
    static const inline auto s_12 = 12;
    static const inline auto s_13 = 17;
    static const inline auto s_14 = 22;
    static const inline auto s_21 = 5;
    static const inline auto s_22 = 9;
    static const inline auto s_23 = 14;
    static const inline auto s_24 = 20;
    static const inline auto s_31 = 4;
    static const inline auto s_32 = 11;
    static const inline auto s_33 = 16;
    static const inline auto s_34 = 23;
    static const inline auto s_41 = 6;
    static const inline auto s_42 = 10;
    static const inline auto s_43 = 15;
    static const inline auto s_44 = 21;    
    
    using digest_type = std::array<u8, 16>;

    struct result_type
    {
        digest_type digest;

        friend bool operator ==(const result_type& lhs, const result_type& rhs)
        {
            return lhs.digest == rhs.digest;
        }

        friend bool operator !=(const result_type& lhs, const result_type& rhs)
        {
            return lhs.digest != rhs.digest;
        }

        friend std::ostream& operator <<(std::ostream& os, const result_type& item)
        {
            for (int ch : item.digest)
            {
                os << std::hex << std::setfill('0') << std::setw(2) << ch;
            }

            return os;
        }
    };

    generator()
        : _finalized{ false }
        , _count{ { 0, 1 } }
        , _state{ { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 } }
        , _buffer{}
        , _digest{}
    {
    }

    void update(const_span<u8> input)
    {
        size_type index = _count[0] / 8 % block_size;

        const auto length = input.size();

        if ((_count[0] += (length << 3)) < (length << 3))
            _count[1]++;

        _count[1] += (length >> 29);

        size_type firstpart = 64 - index;

        size_type i;

        if (length >= firstpart)
        {
            std::memcpy(&_buffer[index], input.data(), firstpart);
            transform(_buffer);

            for (i = firstpart; i + block_size <= length; i += block_size)
                transform({ &input[i], block_size });

            index = 0;
        }
        else
        {
            i = 0;
        }

        std::memcpy(&_buffer[index], &input[i], length - i);
    }

    template <class... Args>
    void operator ()(const Args&... args)
    {
        const auto apply = { 0, (handler<Args>{}(*this, args), 0)... };
        (void)apply;
    }

    generator& finalize()
    {
        static std::array<u8, 64> padding = {
          0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        if (!_finalized)
        {
            std::array<u8, 8> bits;
            encode(bits, _count);

            size_type index = _count[0] / 8 % 64;
            size_type pad_len = (index < 56) ? (56 - index) : (120 - index);
            update(const_span<u8>{ padding.data(), pad_len });

            update(bits);

            encode(_digest, _state);

            std::memset(_buffer.data(), 0, sizeof(_buffer));
            std::memset(_count.data(), 0, sizeof(_count));

            _finalized = true;
        }

        return *this;
    }

    result_type digest()
    {
        finalize();
        return { _digest };
    }

private:
    static constexpr size_type block_size = 64;

    void transform(const_span<u8> block)
    {
        u32 a = _state[0],
            b = _state[1],
            c = _state[2],
            d = _state[3];

        std::array<u32, 16> x = decode(block);

        /* Round 1 */
        calc_ff(a, b, c, d, x[0], s_11, 0xd76aa478); /* 1 */
        calc_ff(d, a, b, c, x[1], s_12, 0xe8c7b756); /* 2 */
        calc_ff(c, d, a, b, x[2], s_13, 0x242070db); /* 3 */
        calc_ff(b, c, d, a, x[3], s_14, 0xc1bdceee); /* 4 */
        calc_ff(a, b, c, d, x[4], s_11, 0xf57c0faf); /* 5 */
        calc_ff(d, a, b, c, x[5], s_12, 0x4787c62a); /* 6 */
        calc_ff(c, d, a, b, x[6], s_13, 0xa8304613); /* 7 */
        calc_ff(b, c, d, a, x[7], s_14, 0xfd469501); /* 8 */
        calc_ff(a, b, c, d, x[8], s_11, 0x698098d8); /* 9 */
        calc_ff(d, a, b, c, x[9], s_12, 0x8b44f7af); /* 10 */
        calc_ff(c, d, a, b, x[10], s_13, 0xffff5bb1); /* 11 */
        calc_ff(b, c, d, a, x[11], s_14, 0x895cd7be); /* 12 */
        calc_ff(a, b, c, d, x[12], s_11, 0x6b901122); /* 13 */
        calc_ff(d, a, b, c, x[13], s_12, 0xfd987193); /* 14 */
        calc_ff(c, d, a, b, x[14], s_13, 0xa679438e); /* 15 */
        calc_ff(b, c, d, a, x[15], s_14, 0x49b40821); /* 16 */

        /* Round 2 */
        calc_gg(a, b, c, d, x[1], s_21, 0xf61e2562); /* 17 */
        calc_gg(d, a, b, c, x[6], s_22, 0xc040b340); /* 18 */
        calc_gg(c, d, a, b, x[11], s_23, 0x265e5a51); /* 19 */
        calc_gg(b, c, d, a, x[0], s_24, 0xe9b6c7aa); /* 20 */
        calc_gg(a, b, c, d, x[5], s_21, 0xd62f105d); /* 21 */
        calc_gg(d, a, b, c, x[10], s_22, 0x2441453); /* 22 */
        calc_gg(c, d, a, b, x[15], s_23, 0xd8a1e681); /* 23 */
        calc_gg(b, c, d, a, x[4], s_24, 0xe7d3fbc8); /* 24 */
        calc_gg(a, b, c, d, x[9], s_21, 0x21e1cde6); /* 25 */
        calc_gg(d, a, b, c, x[14], s_22, 0xc33707d6); /* 26 */
        calc_gg(c, d, a, b, x[3], s_23, 0xf4d50d87); /* 27 */
        calc_gg(b, c, d, a, x[8], s_24, 0x455a14ed); /* 28 */
        calc_gg(a, b, c, d, x[13], s_21, 0xa9e3e905); /* 29 */
        calc_gg(d, a, b, c, x[2], s_22, 0xfcefa3f8); /* 30 */
        calc_gg(c, d, a, b, x[7], s_23, 0x676f02d9); /* 31 */
        calc_gg(b, c, d, a, x[12], s_24, 0x8d2a4c8a); /* 32 */

        /* Round 3 */
        calc_hh(a, b, c, d, x[5], s_31, 0xfffa3942); /* 33 */
        calc_hh(d, a, b, c, x[8], s_32, 0x8771f681); /* 34 */
        calc_hh(c, d, a, b, x[11], s_33, 0x6d9d6122); /* 35 */
        calc_hh(b, c, d, a, x[14], s_34, 0xfde5380c); /* 36 */
        calc_hh(a, b, c, d, x[1], s_31, 0xa4beea44); /* 37 */
        calc_hh(d, a, b, c, x[4], s_32, 0x4bdecfa9); /* 38 */
        calc_hh(c, d, a, b, x[7], s_33, 0xf6bb4b60); /* 39 */
        calc_hh(b, c, d, a, x[10], s_34, 0xbebfbc70); /* 40 */
        calc_hh(a, b, c, d, x[13], s_31, 0x289b7ec6); /* 41 */
        calc_hh(d, a, b, c, x[0], s_32, 0xeaa127fa); /* 42 */
        calc_hh(c, d, a, b, x[3], s_33, 0xd4ef3085); /* 43 */
        calc_hh(b, c, d, a, x[6], s_34, 0x4881d05); /* 44 */
        calc_hh(a, b, c, d, x[9], s_31, 0xd9d4d039); /* 45 */
        calc_hh(d, a, b, c, x[12], s_32, 0xe6db99e5); /* 46 */
        calc_hh(c, d, a, b, x[15], s_33, 0x1fa27cf8); /* 47 */
        calc_hh(b, c, d, a, x[2], s_34, 0xc4ac5665); /* 48 */

        /* Round 4 */
        calc_ii(a, b, c, d, x[0], s_41, 0xf4292244); /* 49 */
        calc_ii(d, a, b, c, x[7], s_42, 0x432aff97); /* 50 */
        calc_ii(c, d, a, b, x[14], s_43, 0xab9423a7); /* 51 */
        calc_ii(b, c, d, a, x[5], s_44, 0xfc93a039); /* 52 */
        calc_ii(a, b, c, d, x[12], s_41, 0x655b59c3); /* 53 */
        calc_ii(d, a, b, c, x[3], s_42, 0x8f0ccc92); /* 54 */
        calc_ii(c, d, a, b, x[10], s_43, 0xffeff47d); /* 55 */
        calc_ii(b, c, d, a, x[1], s_44, 0x85845dd1); /* 56 */
        calc_ii(a, b, c, d, x[8], s_41, 0x6fa87e4f); /* 57 */
        calc_ii(d, a, b, c, x[15], s_42, 0xfe2ce6e0); /* 58 */
        calc_ii(c, d, a, b, x[6], s_43, 0xa3014314); /* 59 */
        calc_ii(b, c, d, a, x[13], s_44, 0x4e0811a1); /* 60 */
        calc_ii(a, b, c, d, x[4], s_41, 0xf7537e82); /* 61 */
        calc_ii(d, a, b, c, x[11], s_42, 0xbd3af235); /* 62 */
        calc_ii(c, d, a, b, x[2], s_43, 0x2ad7d2bb); /* 63 */
        calc_ii(b, c, d, a, x[9], s_44, 0xeb86d391); /* 64 */

        _state[0] += a;
        _state[1] += b;
        _state[2] += c;
        _state[3] += d;

        std::memset(x.data(), 0, sizeof(x));
    }

    static void decode(span<u32> output, const_span<u8> input)
    {
        const auto len = input.size();

        for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
            output[i] = ((u32)input[j]) | (((u32)input[j + 1]) << 8) | (((u32)input[j + 2]) << 16) | (((u32)input[j + 3]) << 24);
    }

    static std::array<u32, 16> decode(const_span<u8> input)
    {
        std::array<u32, 16> result{};
        decode(result, input);
        return result;
    }

    static void encode(span<u8> output, const_span<u32> input)
    {
        const auto len = output.size();
        for (size_type i = 0, j = 0; j < len; i++, j += 4)
        {
            for (int k = 0; k < 4; ++k)
                output[j + k] = (input[i] >> (8 * k)) & 0xff;
        }
    }

    u32 calc_f(u32 x, u32 y, u32 z)
    {
        return x & y | ~x & z;
    }

    u32 calc_g(u32 x, u32 y, u32 z)
    {
        return x & z | y & ~z;
    }

    u32 calc_h(u32 x, u32 y, u32 z)
    {
        return x ^ y ^ z;
    }

    u32 calc_i(u32 x, u32 y, u32 z)
    {
        return y ^ (x | ~z);
    }

    u32 rotate_left(u32 x, int n)
    {
        return (x << n) | (x >> (32 - n));
    }

    void calc_ff(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac)
    {
        a = rotate_left(a + calc_f(b, c, d) + x + ac, s) + b;
    }

    void calc_gg(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac)
    {
        a = rotate_left(a + calc_g(b, c, d) + x + ac, s) + b;
    }

    void calc_hh(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac)
    {
        a = rotate_left(a + calc_h(b, c, d) + x + ac, s) + b;
    }

    void calc_ii(u32& a, u32 b, u32 c, u32 d, u32 x, u32 s, u32 ac)
    {
        a = rotate_left(a + calc_i(b, c, d) + x + ac, s) + b;
    }

    bool _finalized;
    std::array<u32, 2> _count;
    std::array<u32, 4> _state;
    std::array<u8, block_size> _buffer;
    digest_type _digest;
};

template <class... Args>
generator::result_type checksum(const Args&... items)
{
    generator gen{};
    gen(items...);
    return gen.digest();
}

template <class T>
struct handler<T, std::enable_if_t<std::is_arithmetic_v<T>>>
{
    void operator ()(generator& gen, const T& item) const
    {
        gen.update(as_bytes(const_span<T>{ std::addressof(item), 1 }));
    }
};

template <class T>
struct handler<T, std::enable_if_t<std::is_enum_v<T>>>
{
    void operator ()(generator& gen, const T& item) const
    {
        gen((int)item);
    }
};

template <class T, class U>
struct handler<std::pair<T, U>>
{
    void operator ()(generator& gen, const std::pair<T, U>& item) const
    {
        gen(std::get<0>(item), std::get<1>(item));
    }
};

template <class... Args>
struct handler<std::tuple<Args...>>
{
    void operator ()(generator& gen, const std::tuple<Args...>& item) const
    {
        apply(gen, item, std::make_index_sequence<sizeof...(Args)>{});
    }

    template <class Tuple, std::size_t... I>
    void apply(generator& gen, const Tuple& tuple, std::index_sequence<I...>) const
    {
        auto dummy = { 0, (gen(std::get<I>(tuple)), 0)... };
        (void)dummy;
    }
};

template <class T>
struct handler<T, std::void_t<decltype(std::begin(std::declval<T>()))>>
{
    void operator ()(generator& gen, const T& item) const
    {
        for (const auto& elem : item)
            gen(elem);
    }
};

template <class T>
struct handler<std::optional<T>>
{
    void operator ()(generator& gen, const std::optional<T>& item) const
    {
        if (item)
            gen(true, *item);
        else
            gen(false);
    }
};

} // namespace md5
