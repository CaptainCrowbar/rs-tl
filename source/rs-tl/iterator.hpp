#pragma once

#include "rs-tl/types.hpp"
#include <iterator>
#include <type_traits>
#include <utility>

namespace RS::TL {

    // Mixin classes

    template <typename T, typename CV>
    class InputIterator {
    public:
        using difference_type = ptrdiff_t;
        using iterator_category = std::input_iterator_tag;
        using pointer = CV*;
        using reference = CV&;
        using value_type = std::remove_const_t<CV>;
        CV* operator->() const noexcept { return &*static_cast<const T&>(*this); }
        friend T operator++(T& t, int) { T rc = t; ++t; return rc; }
        friend bool operator!=(const T& a, const T& b) noexcept { return ! (a == b); }
    };

    template <typename T>
    class OutputIterator {
    public:
        using difference_type = void;
        using iterator_category = std::output_iterator_tag;
        using pointer = void;
        using reference = void;
        using value_type = void;
        T& operator*() noexcept { return static_cast<T&>(*this); }
        friend T& operator++(T& t) noexcept { return t; }
        friend T operator++(T& t, int) noexcept { return t; }
    };

    template <typename T, typename CV>
    class ForwardIterator:
    public InputIterator<T, CV> {
    public:
        using iterator_category = std::forward_iterator_tag;
    };

    template <typename T, typename CV>
    class BidirectionalIterator:
    public ForwardIterator<T, CV> {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        friend T operator--(T& t, int) { T rc = t; --t; return rc; }
    };

    template <typename T, typename CV>
    class RandomAccessIterator:
    public BidirectionalIterator<T, CV> {
    public:
        // Inheriting from TotalOrder here would lead to function resolution collision for operator!=
        using iterator_category = std::random_access_iterator_tag;
        CV& operator[](ptrdiff_t i) const noexcept { T t = static_cast<const T&>(*this); t += i; return *t; }
        friend T& operator++(T& t) { return t += 1; }
        friend T& operator--(T& t) { return t += -1; }
        friend T& operator-=(T& a, ptrdiff_t b) { return a += - b; }
        friend T operator+(const T& a, ptrdiff_t b) { T t = a; return t += b; }
        friend T operator+(ptrdiff_t a, const T& b) { T t = b; return t += a; }
        friend T operator-(const T& a, ptrdiff_t b) { T t = a; return t -= b; }
        friend bool operator==(const T& a, const T& b) noexcept { return a - b == 0; }
        friend bool operator<(const T& a, const T& b) noexcept { return a - b < 0; }
        friend bool operator>(const T& a, const T& b) noexcept { return b < a; }
        friend bool operator<=(const T& a, const T& b) noexcept { return ! (b < a); }
        friend bool operator>=(const T& a, const T& b) noexcept { return ! (a < b); }
    };

    template <typename T, typename CV, typename Category>
    class FlexibleIterator:
    public InputIterator<T, CV> {
    private:
        static constexpr bool is_random = std::is_same_v<Category, std::random_access_iterator_tag>;
        static constexpr bool is_bidi = is_random || std::is_same_v<Category, std::bidirectional_iterator_tag>;
        template <typename U = T> using BT = std::enable_if_t<SfinaeTrue<U, is_bidi>::value, T>;
        template <typename U = T> using RT = std::enable_if_t<SfinaeTrue<U, is_random>::value, T>;
        template <typename U = T> using RP = std::enable_if_t<SfinaeTrue<U, is_random>::value, ptrdiff_t>;
    public:
        using iterator_category = Category;
        template <typename U = T> CV& operator[](RP<U> i) const noexcept { T t = static_cast<const T&>(*this); t += i; return *t; }
        template <typename U = T> friend T operator--(BT<U>& t, int) { T rc = t; --t; return rc; }
        template <typename U = T> friend T& operator-=(RT<U>& a, ptrdiff_t b) { return a += - b; }
        template <typename U = T> friend T operator+(const RT<U>& a, ptrdiff_t b) { T t = a; return t += b; }
        template <typename U = T> friend T operator+(RP<U> a, const T& b) { T t = b; return t += a; }
        template <typename U = T> friend T operator-(const RT<U>& a, ptrdiff_t b) { T t = a; return t -= b; }
        template <typename U = T> friend bool operator<(const RT<U>& a, const T& b) noexcept { return a - b < 0; }
        template <typename U = T> friend bool operator>(const RT<U>& a, const T& b) noexcept { return b < a; }
        template <typename U = T> friend bool operator<=(const RT<U>& a, const T& b) noexcept { return ! (b < a); }
        template <typename U = T> friend bool operator>=(const RT<U>& a, const T& b) noexcept { return ! (a < b); }
    };

    // Iterator classes

    template <typename Container>
    class AppendIterator:
    public OutputIterator<AppendIterator<Container>> {
    public:
        AppendIterator() = default;
        explicit AppendIterator(Container& c): con_(&c) {}
        AppendIterator& operator=(const typename Container::value_type& x) {
            con_->insert(con_->end(), x);
            return *this;
        }
    private:
        Container* con_;
    };

    template <typename Container>
    AppendIterator<Container> append(Container& c) {
        return AppendIterator<Container>(c);
    }

    template <typename Container>
    AppendIterator<Container> overwrite(Container& c) {
        c.clear();
        return AppendIterator<Container>(c);
    }

    // Range classes

    template <typename Iterator>
    struct Irange {
        Iterator first, second;
        Iterator begin() const { return first; }
        Iterator end() const { return second; }
        bool empty() const noexcept { return first == second; }
    };

    template <typename Iterator>
    Irange<Iterator> irange(Iterator i, Iterator j) {
        return {i, j};
    }

    template <typename Iterator>
    Irange<Iterator> irange(std::pair<Iterator, Iterator> p) {
        return {p.first, p.second};
    }

    template <typename Range>
    Irange<RangeIterator<Range>> subrange(Range& range, int offset) {
        using std::begin;
        using std::end;
        auto i = begin(range);
        auto j = end(range);
        if (offset >= 0) {
            for (; offset > 0 && i != j; ++i, --offset) {}
        } else {
            if constexpr (is_bidirectional_range<Range>) {
                auto k = j;
                for (; offset < 0 && k != i; --k, ++offset) {}
                i = k;
            }
        }
        return {i, j};
    }

    template <typename Range>
    Irange<RangeIterator<Range>> subrange(Range& range, int offset, int offset2) {
        auto sub1 = subrange(range, offset);
        auto sub2 = subrange(sub1, offset2);
        return {sub1.first, sub2.first};
    }

}
