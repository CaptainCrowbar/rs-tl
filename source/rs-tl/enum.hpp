#pragma once

#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

#define RS_DEFINE_ENUM_IMPL_(EnumType, enum_class, IntType, first_value, first_name, ...) \
    enum enum_class EnumType: IntType { \
        first_name = first_value, \
        __VA_ARGS__ \
    }; \
    [[maybe_unused]] inline const std::vector<std::string>& list_enum_names(EnumType) { \
        static const auto names = [] { \
            std::string all = # first_name "," # __VA_ARGS__; \
            std::vector<std::string> vec; \
            size_t i = 0, j = 0; \
            while (j < all.size()) { \
                i = all.find_first_not_of(" ,", j); \
                if (i == std::string::npos) \
                    break; \
                j = all.find_first_of(" ,", i); \
                vec.push_back(all.substr(i, j - i)); \
            } \
            return vec; \
        }(); \
        return names; \
    } \
    [[maybe_unused]] inline const std::vector<EnumType>& list_enum_values(EnumType) { \
        static const auto values = [] { \
            enum class dummy_enum { first_name, __VA_ARGS__, RS_sentinel_ }; \
            auto n = int(dummy_enum::RS_sentinel_); \
            std::vector<EnumType> vec(size_t(n), EnumType(first_value)); \
            for (int i = 0; i < n; ++i) \
                vec[i] = EnumType(first_value + i); \
            return vec; \
        }(); \
        return values; \
    } \
    [[maybe_unused]] inline bool parse_enum(const std::string& str, EnumType& t) { \
        auto& names = list_enum_names(EnumType()); \
        auto it = std::find(names.begin(), names.end(), str); \
        if (it == names.end()) \
            return false; \
        t = EnumType(static_cast<IntType>(first_value) + static_cast<IntType>(it - names.begin())); \
        return true; \
    } \
    [[maybe_unused]] inline std::string to_string(EnumType t) { \
        auto& names = list_enum_names(EnumType()); \
        IntType index = static_cast<IntType>(t) - static_cast<IntType>(first_value); \
        if (index >= 0 && index < static_cast<IntType>(names.size())) \
            return names[size_t(index)]; \
        else \
            return std::to_string(static_cast<IntType>(t)); \
    } \
    [[maybe_unused]] inline std::ostream& operator<<(std::ostream& out, EnumType t) { \
        return out << to_string(t); \
    }

#define RS_DEFINE_ENUM(EnumType, IntType, first_value, first_name, ...) \
    RS_DEFINE_ENUM_IMPL_(EnumType,, IntType, first_value, first_name, __VA_ARGS__)

#define RS_DEFINE_ENUM_CLASS(EnumType, IntType, first_value, first_name, ...) \
    RS_DEFINE_ENUM_IMPL_(EnumType, class, IntType, first_value, first_name, __VA_ARGS__)

#define RS_DEFINE_BITMASK_OPERATORS(EnumClass) \
    [[maybe_unused]] constexpr bool operator!(EnumClass x) noexcept { return x == EnumClass(); } \
    [[maybe_unused]] constexpr EnumClass operator~(EnumClass x) noexcept { \
        using U = std::underlying_type_t<EnumClass>; \
        return EnumClass(~ U(x)); \
    } \
    [[maybe_unused]] constexpr EnumClass operator&(EnumClass x, EnumClass y) noexcept { \
        using U = std::underlying_type_t<EnumClass>; \
        return EnumClass(U(x) & U(y)); \
    } \
    [[maybe_unused]] constexpr EnumClass operator|(EnumClass x, EnumClass y) noexcept { \
        using U = std::underlying_type_t<EnumClass>; \
        return EnumClass(U(x) | U(y)); \
    } \
    [[maybe_unused]] constexpr EnumClass operator^(EnumClass x, EnumClass y) noexcept { \
        using U = std::underlying_type_t<EnumClass>; \
        return EnumClass(U(x) ^ U(y)); \
    } \
    [[maybe_unused]] constexpr EnumClass& operator&=(EnumClass& x, EnumClass y) noexcept { return x = x & y; } \
    [[maybe_unused]] constexpr EnumClass& operator|=(EnumClass& x, EnumClass y) noexcept { return x = x | y; } \
    [[maybe_unused]] constexpr EnumClass& operator^=(EnumClass& x, EnumClass y) noexcept { return x = x ^ y; }
