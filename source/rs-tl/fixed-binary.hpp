#pragma once

#include "rs-tl/binary.hpp"
#include "rs-tl/types.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <initializer_list>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace RS::TL {

    template <size_t N> class SmallBinary;
    template <size_t N> class LargeBinary;
    template <size_t N> using Binary = std::conditional_t<(N <= 64), SmallBinary<N>, LargeBinary<N>>;

    using Uint128 = Binary<128>;
    using Uint256 = Binary<256>;
    using Uint512 = Binary<512>;
    using Uint1024 = Binary<1024>;

    namespace Detail {

        template <typename T>
        std::string to_binary(T t, size_t digits = 8 * sizeof(T)) {
            std::string s(digits, '0');
            for (int i = int(digits) - 1; i >= 0 && t != 0; --i, t /= 2)
                s[i] = char('0' + int(t % 2));
            return s;
        }

        template <typename T>
        std::string to_hex(T t, size_t digits = 2 * sizeof(T)) {
            static constexpr const char* xdigits = "0123456789abcdef";
            std::string s(digits, '0');
            for (int i = int(digits) - 1; i >= 0 && t != 0; --i, t /= 16)
                s[i] = xdigits[t % 16];
            return s;
        }

        template <typename T>
        bool parse_fixed_binary(T& t, const std::string& str, int base, bool check) {

            if (str.empty()) {
                if (check)
                    throw std::invalid_argument("Invalid base " + std::to_string(base) + " number: \"\"");
                return false;
            }

            char max_d = base < 10 ? '0' + base : '9';
            char max_uc = 'A' + base - 11;
            char max_lc = max_uc + 32;
            int digit = 0;
            T result;
            T tbase = T(uint64_t(base));

            for (char c: str) {
                if (c >= '0' && c <= max_d)
                    digit = c - '0';
                else if (c >= 'A' && c <= max_uc)
                    digit = c - 'A' + 10;
                else if (c >= 'a' && c <= max_lc)
                    digit = c - 'a' + 10;
                else if (check)
                    throw std::invalid_argument("Invalid base " + std::to_string(base) + " number: \"" + str + "\"");
                else
                    return false;
                result = tbase * result + T(uint64_t(digit));
            }

            t = std::move(result);

            return true;

        }

    }

    template <size_t N>
    class SmallBinary:
    public TotalOrder<SmallBinary<N>> {

    private:

        static_assert(N >= 1 && N <= 64);

        using value_type = std::conditional_t<(N <= 8), uint8_t,
            std::conditional_t<(N <= 16), uint16_t,
            std::conditional_t<(N <= 32), uint32_t, uint64_t>>>;

    public:

        static constexpr size_t bits = N;
        static constexpr size_t bytes = sizeof(value_type);
        static constexpr size_t hex_digits = (N + 3) / 4;

        constexpr SmallBinary() noexcept: value_(0) {}
        SmallBinary(uint64_t x) noexcept: value_(x & mask) {}
        template <size_t M> explicit SmallBinary(SmallBinary<M> x) noexcept: SmallBinary(uint64_t(x)) {}
        explicit SmallBinary(std::initializer_list<uint64_t> init) noexcept: value_(init.size() ? *init.begin() & mask : 0) {}
        explicit SmallBinary(const std::string& str) { *this = parse_dec(str); }

        std::string bin() const { return Detail::to_binary(value_, N); }
        std::string dec() const { return std::to_string(value_); }
        std::string hex() const { return Detail::to_hex(value_, hex_digits); }
        void clear() noexcept { value_ = 0; }
        int compare(SmallBinary y) const noexcept { return value_ < y.value_ ? -1 : value_ > y.value_ ? 1 : 0; }
        constexpr uint8_t* data() noexcept { return reinterpret_cast<uint8_t*>(&value_); }
        constexpr const uint8_t* data() const noexcept { return reinterpret_cast<const uint8_t*>(&value_); }
        template <typename T> bool fits_in() const noexcept { return significant_bits() <= std::numeric_limits<T>::digits; }
        size_t hash() const noexcept { return std::hash<value_type>()(value_); }
        size_t significant_bits() const noexcept { return bit_width(value_); }

        explicit operator bool() const noexcept { return value_ != 0; }
        template <typename T> explicit operator T() const noexcept { static_assert(std::is_arithmetic_v<T>); return T(value_); }

        SmallBinary operator+() const noexcept { return *this; }
        SmallBinary operator-() const noexcept { auto x = ~ *this; ++x; return x; }
        SmallBinary operator~() const noexcept { return SmallBinary(~ uint64_t(value_)); }
        SmallBinary& operator++() noexcept { ++value_; value_ &= mask; return *this; }
        SmallBinary operator++(int) noexcept { auto x = *this; ++*this; return x; }
        SmallBinary& operator--() noexcept { --value_; value_ &= mask; return *this; }
        SmallBinary operator--(int) noexcept { auto x = *this; --*this; return x; }
        SmallBinary& operator+=(SmallBinary y) noexcept { value_ += y.value_; value_ &= mask; return *this; }
        SmallBinary& operator-=(SmallBinary y) noexcept { value_ -= y.value_; value_ &= mask; return *this; }
        SmallBinary& operator*=(SmallBinary y) noexcept { value_ *= y.value_; value_ &= mask; return *this; }
        SmallBinary& operator/=(SmallBinary y) noexcept { value_ /= y.value_; return *this; }
        SmallBinary& operator%=(SmallBinary y) noexcept { value_ %= y.value_; return *this; }
        SmallBinary& operator&=(SmallBinary y) noexcept { value_ &= y.value_; return *this; }
        SmallBinary& operator|=(SmallBinary y) noexcept { value_ |= y.value_; return *this; }
        SmallBinary& operator^=(SmallBinary y) noexcept { value_ ^= y.value_; return *this; }
        SmallBinary& operator<<=(int y) noexcept
            { if (y < 0) *this >>= - y; else if (size_t(y) < N) value_ = (value_ << y) & mask; else value_ = 0; return *this; }
        SmallBinary& operator>>=(int y) noexcept
            { if (y < 0) *this <<= - y; else if (size_t(y) < N) value_ >>= y; else value_ = 0; return *this; }

        static void divide(SmallBinary x, SmallBinary y, SmallBinary& q, SmallBinary& r) noexcept { q = x / y; r = x % y; }
        static SmallBinary from_double(double x) noexcept { return SmallBinary(uint64_t(x)); }
        static SmallBinary max() noexcept { return SmallBinary(mask); }
        static SmallBinary parse_bin(const std::string& str) { SmallBinary x; Detail::parse_fixed_binary(x, str, 2, true); }
        static SmallBinary parse_dec(const std::string& str) { SmallBinary x; Detail::parse_fixed_binary(x, str, 10, true); }
        static SmallBinary parse_hex(const std::string& str) { SmallBinary x; Detail::parse_fixed_binary(x, str, 16, true); }
        static bool try_parse_bin(const std::string& str, SmallBinary& x) noexcept { return Detail::parse_fixed_binary(x, str, 2, false); }
        static bool try_parse_dec(const std::string& str, SmallBinary& x) noexcept { return Detail::parse_fixed_binary(x, str, 10, false); }
        static bool try_parse_hex(const std::string& str, SmallBinary& x) noexcept { return Detail::parse_fixed_binary(x, str, 16, false); }

        friend SmallBinary rotl(SmallBinary x, int y) noexcept { y %= int(N); return (x << y) | (x >> (int(N) - y)); }
        friend SmallBinary rotr(SmallBinary x, int y) noexcept { y %= int(N); return (x >> y) | (x << (int(N) - y)); }
        friend SmallBinary operator+(SmallBinary x, SmallBinary y) noexcept { auto z = x; z += y; return z; }
        friend SmallBinary operator-(SmallBinary x, SmallBinary y) noexcept { auto z = x; z -= y; return z; }
        friend SmallBinary operator*(SmallBinary x, SmallBinary y) noexcept { auto z = x; z *= y; return z; }
        friend SmallBinary operator/(SmallBinary x, SmallBinary y) noexcept { auto z = x; z /= y; return z; }
        friend SmallBinary operator%(SmallBinary x, SmallBinary y) noexcept { auto z = x; z %= y; return z; }
        friend SmallBinary operator&(SmallBinary x, SmallBinary y) noexcept { auto z = x; z &= y; return z; }
        friend SmallBinary operator|(SmallBinary x, SmallBinary y) noexcept { auto z = x; z |= y; return z; }
        friend SmallBinary operator^(SmallBinary x, SmallBinary y) noexcept { auto z = x; z ^= y; return z; }
        friend SmallBinary operator<<(SmallBinary x, int y) noexcept { auto z = x; z <<= y; return z; }
        friend SmallBinary operator>>(SmallBinary x, int y) noexcept { auto z = x; z >>= y; return z; }
        friend bool operator==(SmallBinary x, SmallBinary y) noexcept { return x.value_ == y.value_; }
        friend bool operator<(SmallBinary x, SmallBinary y) noexcept { return x.value_ < y.value_; }
        friend std::ostream& operator<<(std::ostream& out, SmallBinary x) { return out << x.dec(); }

        friend std::string to_string(SmallBinary x) { return x.dec(); }

    private:

        static constexpr value_type mask = (value_type(1) << N / 2 << (N + 1) / 2) - 1;

        value_type value_;

    };

    template <size_t N>
    class LargeBinary:
    public TotalOrder<LargeBinary<N>> {

    private:

        static_assert(N >= 1);

        using unit_type = uint32_t;

        static constexpr size_t unit_bytes = sizeof(unit_type);
        static constexpr size_t unit_bits = 8 * unit_bytes;
        static constexpr size_t units = (N + unit_bits - 1) / unit_bits;

    public:

        static constexpr size_t bits = N;
        static constexpr size_t bytes = units * unit_bytes;
        static constexpr size_t hex_digits = (N + 3) / 4;

        constexpr LargeBinary() noexcept {}
        LargeBinary(uint64_t x) noexcept;
        explicit LargeBinary(std::initializer_list<uint64_t> init) noexcept;
        explicit LargeBinary(const std::string& str) { *this = parse_dec(str); }
        template <size_t M> explicit LargeBinary(SmallBinary<M> x) noexcept: LargeBinary(uint64_t(x)) {}
        template <size_t M> explicit LargeBinary(const LargeBinary<M>& x) noexcept;

        std::string bin() const;
        std::string dec() const;
        std::string hex() const;
        void clear() noexcept { array_ = {}; }
        int compare(const LargeBinary& y) const noexcept;
        constexpr uint8_t* data() noexcept { return reinterpret_cast<uint8_t*>(array_.data()); }
        constexpr const uint8_t* data() const noexcept { return reinterpret_cast<const uint8_t*>(array_.data()); }
        template <typename T> bool fits_in() const noexcept { return significant_bits() <= std::numeric_limits<T>::digits; }
        size_t hash() const noexcept;
        size_t significant_bits() const noexcept;

        explicit operator bool() const noexcept;
        template <typename T> explicit operator T() const noexcept;
        template <size_t M> explicit operator SmallBinary<M>() const noexcept { return SmallBinary<M>(uint64_t(*this)); }

        LargeBinary operator+() const noexcept { return *this; }
        LargeBinary operator-() const noexcept { auto x = ~ *this; ++x; return x; }
        LargeBinary operator~() const noexcept;
        LargeBinary& operator++() noexcept;
        LargeBinary operator++(int) noexcept { auto x = *this; ++*this; return x; }
        LargeBinary& operator--() noexcept;
        LargeBinary operator--(int) noexcept { auto x = *this; --*this; return x; }
        LargeBinary& operator+=(const LargeBinary& y) noexcept;
        LargeBinary& operator-=(const LargeBinary& y) noexcept;
        LargeBinary& operator*=(const LargeBinary& y) noexcept;
        LargeBinary& operator/=(const LargeBinary& y) noexcept { LargeBinary q, r; divide(*this, y, q, r); *this = q; return *this; }
        LargeBinary& operator%=(const LargeBinary& y) noexcept { LargeBinary q, r; divide(*this, y, q, r); *this = r; return *this; }
        LargeBinary& operator&=(const LargeBinary& y) noexcept;
        LargeBinary& operator|=(const LargeBinary& y) noexcept;
        LargeBinary& operator^=(const LargeBinary& y) noexcept;
        LargeBinary& operator<<=(int y) noexcept;
        LargeBinary& operator>>=(int y) noexcept;

        static void divide(const LargeBinary& x, const LargeBinary& y, LargeBinary& q, LargeBinary& r) noexcept;
        static LargeBinary from_double(double x) noexcept;
        static LargeBinary max() noexcept;
        static LargeBinary parse_bin(const std::string& str) { LargeBinary x; Detail::parse_fixed_binary(x, str, 2, true); }
        static LargeBinary parse_dec(const std::string& str) { LargeBinary x; Detail::parse_fixed_binary(x, str, 10, true); }
        static LargeBinary parse_hex(const std::string& str) { LargeBinary x; Detail::parse_fixed_binary(x, str, 16, true); }
        static bool try_parse_bin(const std::string& str, LargeBinary& x) noexcept { return Detail::parse_fixed_binary(x, str, 2, false); }
        static bool try_parse_dec(const std::string& str, LargeBinary& x) noexcept { return Detail::parse_fixed_binary(x, str, 10, false); }
        static bool try_parse_hex(const std::string& str, LargeBinary& x) noexcept { return Detail::parse_fixed_binary(x, str, 16, false); }

        friend LargeBinary rotl(const LargeBinary& x, int y) noexcept { y %= int(N); return (x << y) | (x >> (int(N) - y)); }
        friend LargeBinary rotr(const LargeBinary& x, int y) noexcept { y %= int(N); return (x >> y) | (x << (int(N) - y)); }
        friend LargeBinary operator+(const LargeBinary& x, const LargeBinary& y) noexcept { auto z = x; z += y; return z; }
        friend LargeBinary operator-(const LargeBinary& x, const LargeBinary& y) noexcept { auto z = x; z -= y; return z; }
        friend LargeBinary operator*(const LargeBinary& x, const LargeBinary& y) noexcept { auto z = x; z *= y; return z; }
        friend LargeBinary operator/(const LargeBinary& x, const LargeBinary& y) noexcept { auto z = x; z /= y; return z; }
        friend LargeBinary operator%(const LargeBinary& x, const LargeBinary& y) noexcept { auto z = x; z %= y; return z; }
        friend LargeBinary operator&(const LargeBinary& x, const LargeBinary& y) noexcept { auto z = x; z &= y; return z; }
        friend LargeBinary operator|(const LargeBinary& x, const LargeBinary& y) noexcept { auto z = x; z |= y; return z; }
        friend LargeBinary operator^(const LargeBinary& x, const LargeBinary& y) noexcept { auto z = x; z ^= y; return z; }
        friend LargeBinary operator<<(const LargeBinary& x, int y) noexcept { auto z = x; z <<= y; return z; }
        friend LargeBinary operator>>(const LargeBinary& x, int y) noexcept { auto z = x; z >>= y; return z; }
        friend bool operator==(const LargeBinary& x, const LargeBinary& y) noexcept { return x.compare(y) == 0; }
        friend bool operator<(const LargeBinary& x, const LargeBinary& y) noexcept { return x.compare(y) < 0; }
        friend std::ostream& operator<<(std::ostream& out, const LargeBinary& x) { return out << x.dec(); }

        friend std::string to_string(LargeBinary x) { return x.dec(); }

    private:

        static constexpr size_t final_bits = N % unit_bits ? N % unit_bits : unit_bits;
        static constexpr size_t final_hex_digits = (final_bits + 3) / 4;
        static constexpr size_t excess_bits = unit_bits - final_bits;
        static constexpr unit_type unit_mask = ~ unit_type(0);
        static constexpr unit_type high_mask = unit_mask >> excess_bits;

        std::array<unit_type, units> array_ = {}; // Little endian order

        void do_mask() noexcept { array_[units - 1] &= high_mask; }

        static void add_with_carry(unit_type& x, unit_type y, unit_type& c) noexcept;

    };

        template <size_t N>
        LargeBinary<N>::LargeBinary(uint64_t x) noexcept {
            array_[0] = unit_type(x);
            if constexpr (units > 1)
                array_[1] = unit_type(x >> unit_bits);
            for (size_t i = 2; i < units; ++i)
                array_[i] = 0;
            do_mask();
        }

        template <size_t N>
        LargeBinary<N>::LargeBinary(std::initializer_list<uint64_t> init) noexcept {
            clear();
            auto ptr = init.begin();
            size_t len = init.size();
            for (size_t i = len - 1, j = 0; i != npos && j < units; --i, j += 2) {
                array_[j] = unit_type(ptr[i]);
                if (j + 1 < units)
                    array_[j + 1] = unit_type(ptr[i] >> unit_bits);
            }
            do_mask();
        }

        template <size_t N>
        template <size_t M>
        LargeBinary<N>::LargeBinary(const LargeBinary<M>& x) noexcept {
            size_t common_units = (std::min(M, N) + unit_bits - 1) / unit_bits;
            auto xdata = reinterpret_cast<const unit_type*>(x.data());
            size_t i = 0;
            for (; i < common_units; ++i)
                array_[i] = xdata[i];
            for (; i < units; ++i)
                array_[i] = 0;
            do_mask();
        }

        template <size_t N>
        std::string LargeBinary<N>::bin() const {
            std::string s;
            s.reserve(N);
            s = Detail::to_binary(array_[units - 1], final_bits);
            for (size_t i = units - 2; i != npos; --i)
                s += Detail::to_binary(array_[i]);
            return s;
        }

        template <size_t N>
        std::string LargeBinary<N>::dec() const {
            std::string s;
            LargeBinary x(*this), base(10), q, r;
            do {
                divide(x, base, q, r);
                s += char(int(r.array_[0]) + '0');
                x = q;
            } while(x);
            std::reverse(s.begin(), s.end());
            return s;
        }

        template <size_t N>
        std::string LargeBinary<N>::hex() const {
            std::string s;
            s.reserve(hex_digits);
            s = Detail::to_hex(array_[units - 1], final_hex_digits);
            for (size_t i = units - 2; i != npos; --i)
                s += Detail::to_hex(array_[i]);
            return s;
        }

        template <size_t N>
        int LargeBinary<N>::compare(const LargeBinary& y) const noexcept {
            for (size_t i = units - 1; i != npos; --i)
                if (array_[i] != y.array_[i])
                    return array_[i] < y.array_[i] ? -1 : 1;
            return 0;
        }

        template <size_t N>
        size_t LargeBinary<N>::hash() const noexcept {
            size_t h = 0;
            for (auto x: array_)
                h ^= (h << 6) + (h >> 2) + std::hash<uint64_t>()(x) + 0x9e3779b9ul;
            return h;
        }

        template <size_t N>
        size_t LargeBinary<N>::significant_bits() const noexcept {
            size_t i = units - 1;
            while (i != npos && array_[i] == 0)
                --i;
            return i == npos ? 0 : unit_bits * i + bit_width(array_[i]);
        }

        template <size_t N>
        LargeBinary<N>::operator bool() const noexcept {
            for (auto u: array_)
                if (u)
                    return true;
            return false;
        }

        template <size_t N>
        template <typename T>
        LargeBinary<N>::operator T() const noexcept {

            static_assert(std::is_arithmetic_v<T>);

            if constexpr (std::is_floating_point_v<T>) {

                size_t i = units - 1;
                while (i != npos && array_[i] == 0)
                    --i;
                if (i == npos)
                    return 0;
                T x = std::ldexp(T(array_[i]), int(unit_bits * i));
                if (i > 0)
                    x += std::ldexp(T(array_[i - 1]), int(unit_bits * (i - 1)));

                return x;

            } else {

                static constexpr size_t t_bits = std::numeric_limits<T>::digits;
                static constexpr size_t t_units = (t_bits + unit_bits - 1) / unit_bits;
                static constexpr size_t n_units = std::min(t_units, units);

                if constexpr (n_units < 2) {
                    return T(array_[0]);
                } else {
                    uintmax_t u = 0;
                    for (int i = int(n_units) - 1; i >= 0; --i)
                        u = (u << unit_bits) + array_[i];
                    return T(u);
                }

            }

        }

        template <size_t N>
        LargeBinary<N> LargeBinary<N>::operator~() const noexcept {
            auto y = *this;
            for (auto& u: y.array_)
                u = ~ u;
            y.do_mask();
            return y;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator++() noexcept {
            ++array_[0];
            for (size_t i = 1; i < units && array_[i - 1] == 0; ++i)
                ++array_[i];
            do_mask();
            return *this;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator--() noexcept {
            --array_[0];
            for (size_t i = 1; i < units && array_[i - 1] == unit_mask; ++i)
                --array_[i];
            do_mask();
            return *this;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator+=(const LargeBinary& y) noexcept {
            unit_type carry = 0;
            for (size_t i = 0; i < units; ++i)
                add_with_carry(array_[i], y.array_[i], carry);
            do_mask();
            return *this;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator-=(const LargeBinary& y) noexcept {
            unit_type borrow = 0, next = 0;
            for (size_t i = 0; i < units; ++i) {
                next = unit_type(array_[i] < y.array_[i] || (array_[i] == y.array_[i] && borrow));
                array_[i] -= y.array_[i] + borrow;
                borrow = next;
            }
            do_mask();
            return *this;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator*=(const LargeBinary& y) noexcept {
            using long_type = uint64_t;
            LargeBinary z;
            for (size_t i = 0; i < units; ++i) {
                for (size_t j = 0; j < units - i; ++j) {
                    size_t k = i + j;
                    long_type p = long_type(array_[i]) * long_type(y.array_[j]);
                    unit_type carry = 0;
                    add_with_carry(z.array_[k], unit_type(p), carry);
                    if (++k < units) {
                        add_with_carry(z.array_[k], unit_type(p >> unit_bits), carry);
                        for (++k; k < units && carry; ++k)
                            add_with_carry(z.array_[k], 0, carry);
                    }
                }
            }
            *this = z;
            do_mask();
            return *this;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator&=(const LargeBinary& y) noexcept {
            for (size_t i = 0; i < units; ++i)
                array_[i] &= y.array_[i];
            return *this;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator|=(const LargeBinary& y) noexcept {
            for (size_t i = 0; i < units; ++i)
                array_[i] |= y.array_[i];
            return *this;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator^=(const LargeBinary& y) noexcept {
            for (size_t i = 0; i < units; ++i)
                array_[i] ^= y.array_[i];
            return *this;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator<<=(int y) noexcept {
            if (y < 0) {
                *this >>= - y;
            } else if (size_t(y) >= N) {
                clear();
            } else {
                size_t skip = size_t(y / unit_bits), keep = units - skip;
                if (skip) {
                    for (size_t i = keep - 1; i != npos; --i)
                        array_[i + skip] = array_[i];
                    for (size_t i = 0; i < skip; ++i)
                        array_[i] = 0;
                    y %= unit_bits;
                }
                unit_type carry = 0, next = 0;
                if (y) {
                    for (size_t i = skip; i < units; ++i) {
                        next = array_[i] >> (unit_bits - y);
                        array_[i] <<= y;
                        array_[i] += carry;
                        carry = next;
                    }
                }
                do_mask();
            }
            return *this;
        }

        template <size_t N>
        LargeBinary<N>& LargeBinary<N>::operator>>=(int y) noexcept {
            if (y < 0) {
                *this <<= - y;
            } else if (size_t(y) >= N) {
                clear();
            } else {
                size_t skip = size_t(y / unit_bits), keep = units - skip;
                if (skip) {
                    for (size_t i = 0; i < keep; ++i)
                        array_[i] = array_[i + skip];
                    for (size_t i = keep; i < units; ++i)
                        array_[i] = 0;
                    y %= unit_bits;
                }
                unit_type carry = 0, next = 0;
                if (y) {
                    for (size_t i = keep - 1; i != npos; --i) {
                        next = array_[i] << (unit_bits - y);
                        array_[i] >>= y;
                        array_[i] += carry;
                        carry = next;
                    }
                }
            }
            return *this;
        }

        template <size_t N>
        void LargeBinary<N>::divide(const LargeBinary& x, const LargeBinary& y, LargeBinary& q, LargeBinary& r) noexcept {
            q.clear();
            r = x;
            if (x < y)
                return;
            int shift = int(x.significant_bits()) - int(y.significant_bits());
            auto a = y << shift, b = LargeBinary(1) << shift;
            for (int i = 0; i <= shift && r; ++i, a >>= 1, b >>= 1) {
                if (a <= r) {
                    r -= a;
                    q += b;
                }
            }
        }

        template <size_t N>
        LargeBinary<N> LargeBinary<N>::from_double(double x) noexcept {
            int exp = 0;
            double sig = std::frexp(x, &exp);
            int shift = std::min(int(N), 63);
            sig = std::ldexp(sig, shift);
            exp -= shift;
            auto num = LargeBinary(uint64_t(sig));
            num <<= exp;
            return num;
        }

        template <size_t N>
        LargeBinary<N> LargeBinary<N>::max() noexcept {
            LargeBinary x;
            for (auto& u: x.array_)
                u = unit_mask;
            x.do_mask();
            return x;
        }

        template <size_t N>
        void LargeBinary<N>::add_with_carry(unit_type& x, unit_type y, unit_type& c) noexcept {
            x += y + c;
            c = unit_type(x < y || (x == y && c));
        }

}

namespace std {

    template <size_t N>
    struct hash<RS::TL::SmallBinary<N>> {
        size_t operator()(const RS::TL::SmallBinary<N>& x) const noexcept {
            return x.hash();
        }
    };

    template <size_t N>
    struct hash<RS::TL::LargeBinary<N>> {
        size_t operator()(const RS::TL::LargeBinary<N>& x) const noexcept {
            return x.hash();
        }
    };

}
