#pragma once

namespace alone::shared {
    constexpr bool is_alpha(const char& c) {
        return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
    }
    constexpr bool is_binary(const char& c) {
        return c == '0' || c == '1';
    }
    constexpr bool is_numeric(const char& c) {
        return c >= '0' && c <= '9';
    }
    constexpr bool is_hexadecimal(const char& c) {
        return is_numeric(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
    }
    constexpr bool is_alpha_numeric(const char& c) {
        return is_alpha(c) || is_numeric(c);
    }
    constexpr bool is_whitespace(const char& c) {
        return c == ' ' || c == '\n' || c == '\t';
    }
}
