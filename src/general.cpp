#include "general.hpp"

#include <algorithm>

//alone
#include "error_codes.hpp"

namespace alone::lib {
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

    literal_type check_type(const std::string& l) {
        literal_type result;

        if (is_alpha(l.front())) {
            for (auto c : l | std::views::drop(1)) {
                if (!is_alpha_numeric(c))
                    throw GENERAL_WRONG_WORD_VALUE;
            }
            result = literal_type::word;
        } else if (is_numeric(l.front())) {
            if (l.contains('.')) {
                size_t i;
                for (i = 1; l[i] != '.'; ++i) {
                    if (!is_numeric(l[i]))
                        throw GENERAL_WRONG_FLOATING_VALUE;
                }
                for (++i; i != l.size(); ++i) {
                    if (!is_numeric(l[i]))
                        throw GENERAL_WRONG_FLOATING_VALUE_AFTER_DOT;
                }
                result = literal_type::floating;
            } else if (l.starts_with("0b")) {
                for (auto c : l | std::views::drop(2)) {
                    if (c != '0' && c != '1')
                        throw GENERAL_WRONG_BINARY_VALUE;
                }
                result = literal_type::binary;
            } else if (l.starts_with("0x")) {
                for (auto c : l | std::views::drop(2)) {
                    if (!is_hexadecimal(c))
                        throw GENERAL_WRONG_HEX_VALUE;
                }
                result = literal_type::hexadecimal;
            } else {
                for (auto c : l) {
                    if (!is_numeric(c))
                        throw GENERAL_WRONG_DECIMAL_VALUE;
                }
                result = literal_type::decimal;
            }
        } else
            result = literal_type::none;

        return result;
    }

    inline std::string gen_str(const char& a, const char& b) {
        std::string result;
        result.resize(b - a + 1);
        for (char i = a; i <= b; ++i)
            result[i - a] = i;
        return result;
    }

    std::tuple<address_t, memory_type> decompose_address(const address_t& address) {
        return {
            (address & 0x3fffffffffffffff),
            memory_type((address & 0xC000000000000000) >> (sizeof(address_t) * 8 - 2))
        };
    }
    std::tuple<inst_code_t, args_data_t> decompose_instruction(const inst_code_t& instruction) {
        const size_t args_metadata = (instruction & 0xFF000000) >> 24;
        std::array args = {
            argument_type((args_metadata & 0b00000011) >> 0),
            argument_type((args_metadata & 0b00001100) >> 2),
            argument_type((args_metadata & 0b00110000) >> 4),
            argument_type((args_metadata & 0b11000000) >> 6)
        };
        return { instruction & 0x00FFFFFF, args };
    }
}
