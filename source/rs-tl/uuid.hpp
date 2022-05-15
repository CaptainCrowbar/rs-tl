#pragma once

#include "rs-tl/types.hpp"
#include <algorithm>
#include <array>
#include <ostream>
#include <stdexcept>
#include <string>

namespace RS::TL {

    namespace Detail {

        constexpr uint32_t bernstein_hash(const void* ptr, size_t len) noexcept {
            auto bptr = static_cast<const uint8_t*>(ptr);
            uint32_t h = 5381;
            for (size_t i = 0; i < len; ++i)
                h = 33 * h + bptr[i];
            return h;
        }

    }

    class Uuid:
    public TotalOrder<Uuid> {

    public:

        constexpr Uuid() noexcept: bytes_{} { for (auto& b: bytes_) b = 0; }

        constexpr Uuid(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g, uint8_t h,
            uint8_t i, uint8_t j, uint8_t k, uint8_t l, uint8_t m, uint8_t n, uint8_t o, uint8_t p) noexcept:
        bytes_{{a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p}} {}

        constexpr Uuid(const void* ptr, size_t len) noexcept:
        bytes_{} {
            auto bptr = static_cast<const uint8_t*>(ptr);
            if (len > 16)
                len = 16;
            for (size_t i = 0; i < len; ++i)
                bytes_[i] = bptr[i];
            for (size_t i = len; i < 16; ++i)
                bytes_[i] = 0;
        }

        explicit Uuid(const std::string& str) {
            auto s = str.begin();
            auto s_end = str.end();
            auto t = begin();
            auto t_end = end();
            while (s != s_end && t != t_end) {
                s = std::find_if(s, s_end, is_alnum);
                if (s == s_end)
                    break;
                if (s[0] == '0' && (s[1] == 'X' || s[1] == 'x')) {
                    s += 2;
                    continue;
                }
                if (s == s_end || ! is_xdigit(s[0]) || ! is_xdigit(s[1]))
                    break;
                int x = 0;
                for (int i = 0; i < 2; ++i) {
                    x *= 16;
                    if (s[i] <= '9')
                        x += s[i] - '0';
                    else if (s[i] <= 'Z')
                        x += s[i] - 'A' + 10;
                    else
                        x += s[i] - 'a' + 10;
                }
                *t++ = uint8_t(x);
                s += 2;
            }
            if (t == t_end)
                s = std::find_if(s, s_end, is_alnum);
            if (s != s_end || t != t_end)
                throw std::invalid_argument("Invalid UUID: " + str);
        }

        constexpr uint8_t& operator[](size_t i) noexcept { return bytes_[i]; }
        constexpr const uint8_t& operator[](size_t i) const noexcept { return bytes_[i]; }
        constexpr uint8_t* begin() noexcept { return bytes_.data(); }
        constexpr const uint8_t* begin() const noexcept { return bytes_.data(); }
        constexpr uint8_t* end() noexcept { return begin() + 16; }
        constexpr const uint8_t* end() const noexcept { return begin() + 16; }
        constexpr size_t hash() const noexcept { return Detail::bernstein_hash(begin(), 16); }

        std::string str() const {
            std::string str;
            str.reserve(36);
            int i = 0;
            for (; i < 4; ++i)
                add_byte(bytes_[i], str);
            str += '-';
            for (; i < 6; ++i)
                add_byte(bytes_[i], str);
            str += '-';
            for (; i < 8; ++i)
                add_byte(bytes_[i], str);
            str += '-';
            for (; i < 10; ++i)
                add_byte(bytes_[i], str);
            str += '-';
            for (; i < 16; ++i)
                add_byte(bytes_[i], str);
            return str;
        }

        friend constexpr bool operator==(const Uuid& u, const Uuid& v) noexcept {
            for (int i = 0; i < 16; ++i)
                if (u.bytes_[i] != v.bytes_[i])
                    return false;
            return true;
        }

        friend constexpr bool operator<(const Uuid& u, const Uuid& v) noexcept {
            for (int i = 0; i < 16; ++i)
                if (u.bytes_[i] != v.bytes_[i])
                    return u.bytes_[i] < v.bytes_[i];
            return false;
        }

        friend std::ostream& operator<<(std::ostream& out, const Uuid& u) { return out << u.str(); }

    private:

        std::array<uint8_t, 16> bytes_;

        static void add_byte(uint8_t b, std::string& str) {
            static constexpr const char* xdigits = "0123456789abcdef";
            str += xdigits[b / 16];
            str += xdigits[b % 16];
        }

        constexpr static bool is_alnum(char c) noexcept { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
        constexpr static bool is_xdigit(char c) noexcept { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }

    };

}

namespace std {

    template <>
    class hash<RS::TL::Uuid> {
        size_t operator()(const RS::TL::Uuid& u) const noexcept {
            return u.hash();
        }
    };

}
