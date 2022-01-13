#pragma once

#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

namespace RS::TL {

    // Constants

    constexpr size_t npos = std::string::npos;

    // Mixin classes

    template <typename T>
    struct TotalOrder {
        friend bool operator!=(const T& a, const T& b) noexcept { return ! (a == b); }
        friend bool operator>(const T& a, const T& b) noexcept { return b < a; }
        friend bool operator<=(const T& a, const T& b) noexcept { return ! (b < a); }
        friend bool operator>=(const T& a, const T& b) noexcept { return ! (a < b); }
    };

    // Static assert support

    template <typename T> constexpr bool dependent_false = false;

    // Type traits

    namespace Detail {

        template <typename T, typename = void> struct HasIteratorCategory: std::false_type {};
        template <typename T> struct HasIteratorCategory<T, std::void_t<typename std::iterator_traits<T>::iterator_category>>: std::true_type {};
        template <typename T, typename = void> struct HasAdlBeginFunction: std::false_type {};
        template <typename T> struct HasAdlBeginFunction<T, std::void_t<decltype(begin(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasAdlEndFunction: std::false_type {};
        template <typename T> struct HasAdlEndFunction<T, std::void_t<decltype(end(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasStdBeginFunction: std::false_type {};
        template <typename T> struct HasStdBeginFunction<T, std::void_t<decltype(std::begin(std::declval<const T&>()))>>: std::true_type {};
        template <typename T, typename = void> struct HasStdEndFunction: std::false_type {};
        template <typename T> struct HasStdEndFunction<T, std::void_t<decltype(std::end(std::declval<const T&>()))>>: std::true_type {};

        template <typename T, bool UseAdl = HasAdlBeginFunction<T>::value, bool UseStd = HasStdBeginFunction<T>::value> struct RangeTraits {
            using iterator_type = void;
            using value_type = void;
        };
        template <typename T, bool UseStd> struct RangeTraits<T, true, UseStd> {
            using iterator_type = std::decay_t<decltype(begin(std::declval<T&>()))>;
            using value_type = std::decay_t<decltype(*std::declval<iterator_type>())>;
        };
        template <typename T> struct RangeTraits<T, false, true> {
            using iterator_type = std::decay_t<decltype(std::begin(std::declval<T&>()))>;
            using value_type = std::decay_t<decltype(*std::declval<iterator_type>())>;
        };

    }

    template <typename T> constexpr bool is_iterator = Detail::HasIteratorCategory<T>::value;
    template <typename T> constexpr bool is_range = (Detail::HasAdlBeginFunction<T>::value && Detail::HasAdlEndFunction<T>::value)
        || (Detail::HasStdBeginFunction<T>::value && Detail::HasStdEndFunction<T>::value);

    template <typename T> using RangeIterator = typename Detail::RangeTraits<T>::iterator_type;
    template <typename T> using RangeValue = typename Detail::RangeTraits<T>::value_type;

}
