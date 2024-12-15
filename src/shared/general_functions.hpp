#pragma once

namespace alone::shared {
    constexpr bool is_alpha(const char& c);
    constexpr bool is_binary(const char& c);
    constexpr bool is_numeric(const char& c);
    constexpr bool is_hexadecimal(const char& c);
    constexpr bool is_alpha_numeric(const char& c);
    constexpr bool is_whitespace(const char& c);
}
