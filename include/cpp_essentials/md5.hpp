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
#include <algorithm>

namespace md5
{

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

	using u8 = unsigned char;
	using u32 = std::uint32_t;

	using size_type = unsigned int;
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
			os << std::hex << std::setfill('0') << std::setw(2);

			std::copy(
				item.digest.begin(),
				item.digest.end(),
				std::ostream_iterator<int>{ os, "" });

			return os;
		}
	};

	generator()
	{
		init();
	}

	void update(const u8* input, size_type length)
	{
		size_type index = _count[0] / 8 % blocksize;

		if ((_count[0] += (length << 3)) < (length << 3))
			_count[1]++;

		_count[1] += (length >> 29);

		size_type firstpart = 64 - index;

		size_type i;

		if (length >= firstpart)
		{
			memcpy(&_buffer[index], input, firstpart);
			transform(_buffer.data());

			for (i = firstpart; i + blocksize <= length; i += blocksize)
				transform(&input[i]);

			index = 0;
		}
		else
		{
			i = 0;
		}

		memcpy(&_buffer[index], &input[i], length - i);
	}

	void update(const void* input, size_type length)
	{
		update((const u8*)input, length);
	}

	template <class... Args>
	void operator ()(const Args&... args)
	{
		const auto apply = { 0, (handler<Args>{}(*this, args), 0)... };
		(void)apply;
	}

	generator& finalize()
	{
		static u8 padding[64] = {
		  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		if (!_finalized)
		{
			u8 bits[8];
			encode(bits, _count.data(), 8);

			size_type index = _count[0] / 8 % 64;
			size_type padLen = (index < 56) ? (56 - index) : (120 - index);
			update(padding, padLen);

			update(bits, 8);

			encode(_digest.data(), _state.data(), 16);

			memset(_buffer.data(), 0, sizeof(_buffer));
			memset(_count.data(), 0, sizeof(_count));

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
	static constexpr auto blocksize = 64;

	void init()
	{
		_finalized = false;

		_count[0] = 0;
		_count[1] = 0;

		_state[0] = 0x67452301;
		_state[1] = 0xefcdab89;
		_state[2] = 0x98badcfe;
		_state[3] = 0x10325476;
	}	

	void transform(const u8 block[blocksize])
	{
		u32 a = _state[0], b = _state[1], c = _state[2], d = _state[3];
		std::array<u32, 16> x;
		decode(x.data(), block, blocksize);

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

		memset(x.data(), 0, sizeof(x));
	}

	static void decode(u32* output, const u8* input, size_type len)
	{
		for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
			output[i] = ((u32)input[j]) | (((u32)input[j + 1]) << 8) | (((u32)input[j + 2]) << 16) | (((u32)input[j + 3]) << 24);
	}

	static void encode(u8* output, const u32* input, size_type len)
	{
		for (size_type i = 0, j = 0; j < len; i++, j += 4)
		{
			output[j + 0] = (input[i] >> 0) & 0xff;
			output[j + 1] = (input[i] >> 8) & 0xff;
			output[j + 2] = (input[i] >> 16) & 0xff;
			output[j + 3] = (input[i] >> 24) & 0xff;
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
	std::array<u8, blocksize> _buffer;
	std::array<u32, 2> _count;
	std::array<u32, 4> _state;
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
		gen.update(std::addressof(item), (generator::size_type)sizeof(item));
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

} // namespace md5
