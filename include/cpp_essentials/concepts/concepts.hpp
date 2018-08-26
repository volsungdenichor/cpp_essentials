#ifndef CPP_ESSENTIALS_CONCEPTS_CONCEPTS_HPP_
#define CPP_ESSENTIALS_CONCEPTS_CONCEPTS_HPP_

#include <type_traits>
#include <iterator>
#include <cassert>
#include <vector>

#if defined(__MINGW32__)
#  define CONCEPT_IF(...) class = std::enable_if_t<(__VA_ARGS__)>
#  define CONCEPT_CHECK(CONDITION) static_assert(CONDITION, "condition not met: " #CONDITION)
#elif defined(__GNUC__)
#  define CONCEPT_IF(...) class = std::enable_if_t<(__VA_ARGS__)>
#  define CONCEPT_CHECK(CONDITION) static_assert(CONDITION, "condition not met: " #CONDITION)
#elif defined(_MSC_VER)
#  define CONCEPT_IF(...) class = std::enable_if_t<(__VA_ARGS__)>
#  define CONCEPT_CHECK(CONDITION) static_assert(CONDITION, "condition not met: " #CONDITION)
#elif defined(__clang__)
#  define CONCEPT_IF(...) class = std::enable_if_t<(__VA_ARGS__)>
#  define CONCEPT_CHECK(CONDITION) static_assert(CONDITION, "condition not met: " #CONDITION)
#endif

namespace concepts
{

template <bool Value>
using boolean_constant = std::integral_constant<bool, Value>;

template <class...>
using void_t = void;

namespace detail
{

template <template <class...> class Op, class Enabler, class... Args>
struct is_detected : std::false_type {};

template <template <class...> class Op, class... Args>
struct is_detected<Op, std::void_t<Op<Args...>>, Args...> : std::true_type {};

} /* namespace detail */

template <template <class...> class Op, class... Args>
using is_detected = typename detail::is_detected<Op, void, Args...>::type;



template <class T, class U>
using addition_result = decltype(std::declval<T&>() + std::declval<U&>());

template <class T, class U>
using subtraction_result = decltype(std::declval<T&>() - std::declval<U&>());

template <class T, class U >
using multiplication_result = decltype(std::declval<T&>() * std::declval<U&>());

template <class T, class U>
using division_result = decltype(std::declval<T&>() / std::declval<U&>());

template <class T>
using dereference_result = decltype(*std::declval<T&>());



template <class T, class U, class = void>
struct addable : std::false_type {};

template <class T, class U>
struct addable<T, U, void_t<addition_result<T, U>>> : std::true_type {};

template <class T>
struct addable<T, void> : addable<T, T> {};



template <class T, class U, class = void>
struct subtractable : std::false_type {};

template <class T, class U>
struct subtractable<T, U, void_t<subtraction_result<T, U>>> : std::true_type {};

template <class T>
struct subtractable<T, void> : subtractable<T, T> {};



template <class T, class U, class = void>
struct multipliable : std::false_type {};

template <class T, class U>
struct multipliable<T, U, void_t<multiplication_result<T, U>>> : std::true_type {};

template <class T>
struct multipliable<T, void> : multipliable<T, T> {};



template <class T, class U, class = void>
struct dividable : std::false_type {};

template <class T, class U>
struct dividable<T, U, void_t<division_result<T, U>>> : std::true_type {};

template <class T>
struct dividable<T, void> : dividable<T, T> {};



template <class T, class U, class = void>
struct equality_comparable : std::false_type {};

template <class T, class U>
struct equality_comparable<T, U, void_t<decltype(std::declval<T&>() == std::declval<U&>())>> : std::true_type {};

template <class T>
struct equality_comparable<T, void> : equality_comparable<T, T> {};



template <class T, class U, class = void>
struct less_than_comparable : std::false_type {};

template <class T, class U>
struct less_than_comparable<T, U, void_t<decltype(std::declval<T&>() < std::declval<U&>())>> : std::true_type {};

template <class T>
struct less_than_comparable<T, void> : less_than_comparable<T, T> {};



template <class T, class = void>
struct preincrementable : std::false_type {};

template <class T>
struct preincrementable<T, void_t<decltype(++std::declval<T&>())>> : std::true_type {};



template <class T, class = void>
struct postincrementable : std::false_type {};

template <class T>
struct postincrementable<T, void_t<decltype(std::declval<T&>()++)>> : std::true_type {};



template <class T, class = void>
struct predecrementable : std::false_type {};

template <class T>
struct predecrementable<T, void_t<decltype(--std::declval<T&>())>> : std::true_type {};



template <class T, class = void>
struct postdecrementable : std::false_type {};

template <class T>
struct postdecrementable<T, void_t<decltype(std::declval<T&>()--)>> : std::true_type {};



template <class T, class = void>
struct dereferencable : std::false_type {};

template <class T>
struct dereferencable<T, void_t<dereference_result<T>>> : std::true_type {};



template <class T>
using iterator_category = typename std::iterator_traits<T>::iterator_category;

template <class T>
using iterator_value = typename std::iterator_traits<T>::value_type;

template <class T>
using iterator_reference = typename std::iterator_traits<T>::reference;

template <class T>
using iterator_difference = typename std::iterator_traits<T>::difference_type;

template <class T>
using iterator_pointer = typename std::iterator_traits<T>::pointer;



namespace detail
{

template <typename T>
constexpr bool _is_non_const_reference = std::is_lvalue_reference<T>::value && !std::is_const<std::remove_reference_t<T>>::value;



template <class T, class Category>
struct has_category : boolean_constant<std::is_base_of<Category, iterator_category<T>>::value> {};

template <class T>
struct has_category<T, void> : std::true_type {};

template <class T>
constexpr bool _is_output_iterator =
    std::is_same<std::output_iterator_tag, iterator_category<T>>::value
    || _is_non_const_reference<iterator_reference<T>>
    || std::is_same<T, std::vector<bool>::iterator>::value;

template <class T>
struct has_category<T, std::output_iterator_tag> : boolean_constant<_is_output_iterator<T>> {};



template <class T>
using get_begin = decltype(std::begin(std::declval<T&>()));

template <class T>
using get_end = decltype(std::end(std::declval<T&>()));

} /* namespace detail */



template <class T, class Category = void, class = void>
struct iterator : std::false_type {};

template <class T, class Category>
struct iterator<
    T,
    Category,
    void_t<
        iterator_category<T>,
        iterator_reference<T>,
        iterator_value<T>,
        iterator_pointer<T>,
        iterator_difference<T>,
        std::enable_if_t<detail::has_category<T, Category>::value>>>
    : std::true_type {};



template <class T, class Category = void, class = void>
struct range : std::false_type {};

template <class T, class Category>
struct range<
    T,
    Category,
    void_t<
        std::enable_if_t<iterator<detail::get_begin<T>, Category>::value>,
        std::enable_if_t<iterator<detail::get_end<T>, Category>::value>>>
    : std::true_type {};



template <class T>
struct range_traits
{
    static_assert(range<T>::value, "range required");

    using iterator = detail::get_begin<T>;

    using iterator_category = concepts::iterator_category<iterator>;
    using value_type = concepts::iterator_value<iterator>;
    using reference = concepts::iterator_reference<iterator>;
    using difference_type = concepts::iterator_difference<iterator>;
    using pointer = concepts::iterator_pointer<iterator>;
};

template <class T>
using range_iterator = typename range_traits<T>::iterator;

template <class T>
using range_category = typename range_traits<T>::iterator_category;

template <class T>
using range_value = typename range_traits<T>::value_type;

template <class T>
using range_reference = typename range_traits<T>::reference;

template <class T>
using range_difference = typename range_traits<T>::difference_type;

template <class T>
using range_pointer = typename range_traits<T>::pointer;




template <class T>
using input_iterator = iterator<T, std::input_iterator_tag>;

template <class T>
using forward_iterator = iterator<T, std::forward_iterator_tag>;

template <class T>
using bidirectional_iterator = iterator<T, std::bidirectional_iterator_tag>;

template <class T>
using random_access_iterator = iterator<T, std::random_access_iterator_tag>;

template <class T>
using output_iterator = iterator<T, std::output_iterator_tag>;



template <class T>
using input_range = range<T, std::input_iterator_tag>;

template <class T>
using forward_range = range<T, std::forward_iterator_tag>;

template <class T>
using bidirectional_range = range<T, std::bidirectional_iterator_tag>;

template <class T>
using random_access_range = range<T, std::random_access_iterator_tag>;



template <class T, class = void>
struct has_ostream_operator : std::false_type {};

template <class T>
struct has_ostream_operator<T, void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>> : std::true_type {};



template <class T>
constexpr bool Iterator = iterator<T, void>::value;

template <class T>
constexpr bool InputIterator = input_iterator<T>::value;

template <class T>
constexpr bool ForwardIterator = forward_iterator<T>::value;

template <class T>
constexpr bool BidirectionalIterator = bidirectional_iterator<T>::value;

template <class T>
constexpr bool RandomAccessIterator = random_access_iterator<T>::value;

template <class T>
constexpr bool OutputIterator = output_iterator<T>::value;



template <class T>
constexpr bool Range = range<T>::value;

template <class T>
constexpr bool InputRange = input_range<T>::value;

template <class T>
constexpr bool ForwardRange = forward_range<T>::value;

template <class T>
constexpr bool BidirectionalRange = bidirectional_range<T>::value;

template <class T>
constexpr bool RandomAccessRange = random_access_range<T>::value;



template <class T, class U = void>
constexpr bool Addable = addable<T, U>::value;

template <class T, class U = void>
constexpr bool Subtractable = subtractable<T, U>::value;

template <class T, class U = void>
constexpr bool Multipliable = multipliable<T, U>::value;

template <class T, class U = void>
constexpr bool Dividable = dividable<T, U>::value;

template <class T, class U = void>
constexpr bool EqualityComparable = equality_comparable<T, U>::value;

template <class T, class U = void>
constexpr bool LessThanComparable = less_than_comparable<T, U>::value;

template <class T>
constexpr bool Preincrementable = preincrementable<T>::value;

template <class T>
constexpr bool Postincrementable = postincrementable<T>::value;

template <class T>
constexpr bool Predecrementable = predecrementable<T>::value;

template <class T>
constexpr bool Postdecrementable = postdecrementable<T>::value;

template <class T>
constexpr bool Dereferencable = dereferencable<T>::value;



template <class From, class To>
constexpr bool Convertible = std::is_convertible<From, To>::value;



template <class T>
constexpr bool HasOstreamOperator = has_ostream_operator<T>::value;



template <class T>
constexpr bool Arithmetic = std::is_arithmetic<T>::value;



template <class T, class... Types>
struct is_any_of : std::false_type {};

template <class T, class Arg, class... Args>
struct is_any_of<T, Arg, Args...> : std::integral_constant<bool,
    std::is_same<T, Arg>::value || is_any_of<T, Args...>::value> {};

} /* namespace concepts */

#endif /* CPP_ESSENTIALS_CONCEPTS_CONCEPTS_HPP_ */
