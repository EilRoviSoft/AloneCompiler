#pragma once

//std
#include <functional>
#include <string>

namespace amasm::shared::ext {
#include "shared/crc64.hpp"
}

namespace amasm::shared {
    inline size_t hash_string(const std::string& what) {
        return ext::crc64(0, what.data(), what.size());
    }
    constexpr size_t hash_cstr(const char* what, size_t length) {
        return ext::crc64(0, what, length);
    }

    constexpr bool is_alpha(char c) {
        return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
    }
    constexpr bool is_binary(char c) {
        return c == '0' || c == '1';
    }
    constexpr bool is_numeric(char c) {
        return c >= '0' && c <= '9';
    }
    constexpr bool is_hexadecimal(char c) {
        return is_numeric(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
    }
    constexpr bool is_alpha_numeric(char c) {
        return is_alpha(c) || is_numeric(c);
    }
    constexpr bool is_whitespace(char c) {
        return c == ' ' || c == '\n' || c == '\t';
    }

    template<typename T, typename TBinaryOp, typename TRule>
    T for_each_by_rule(std::ranges::iterator_t<T> first,
                       std::ranges::iterator_t<T> last,
                       T init,
                       TBinaryOp bop,
                       TRule rule) {
        while (first != last) {
            T temp = rule(first);
            init = bop(std::move(init), temp);
        }
        return init;
    }

    template<typename T, std::ranges::input_range TRange, typename TBinaryOp, typename TRule>
    T for_each_by_rule(const TRange& range,
                       T init,
                       TBinaryOp bop,
                       TRule rule) {
        for (const auto& it : range) {
            T temp = rule(it);
            init = bop(std::move(init), temp);
        }
        return init;
    }

    template<typename TOut, typename TIn>
    TOut view_as(TIn* ptr) {
        return reinterpret_cast<TOut>(ptr);
    }

    template<typename T>
    struct bit_shl {
        constexpr auto operator()(T lhs, T rhs) const {
            return lhs << rhs;
        }
    };
    template<typename T>
    struct bit_shr {
        constexpr auto operator()(T lhs, T rhs) const {
            return lhs >> rhs;
        }
    };
}
