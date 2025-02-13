#pragma once

//std
#include <cstdint>
#include <string>
#include <stdexcept>

//lib
#include "library/general_functions.hpp"

namespace amasm::lib {
    enum class LiteralType : uint8_t {
        None = 0,
        Word,
        Binary,
        Decimal,
        Hexadecimal,
        Float
    };

    inline LiteralType check_literal_type(const std::string& str) {
        //check whether it is word
        if (is_alpha(str.front())) {
            for (size_t i = 1; i < str.size(); i++)
                if (!is_alpha_numeric(str[i])) {
                    throw std::runtime_error("wrong word value");
                }
            return LiteralType::Word;
        }

        //wtf is that?
        if (!is_numeric(str.front()))
            return LiteralType::None;

        //check whether it is floating number
        if (str.contains('.')) {
            size_t i;
            for (i = 1; str[i] != '.'; i++)
                if (!is_numeric(str[i])) {
                    throw std::runtime_error("wrong floating value");
                }
            for (i++; i < str.size(); i++)
                if (!is_numeric(str[i])) {
                    throw std::runtime_error("wrong floating value after '.'");
                }
            return LiteralType::Float;
        }

        //check whether it is binary number
        if (str.starts_with("0b")) {
            for (size_t i = 2; i < str.size(); i++)
                if (!is_binary(str[i])) {
                    throw std::runtime_error("wrong binary value");
                }
            return LiteralType::Binary;
        }

        //check whether it is hexadecimal number
        if (str.starts_with("0x")) {
            for (size_t i = 2; i < str.size(); i++)
                if (!is_hexadecimal(str[i])) {
                    throw std::runtime_error("wrong hex value");
                }
            return LiteralType::Hexadecimal;
        }

        //check whether it is integer number
        for (char i : str)
            if (!is_numeric(i)) {
                throw std::runtime_error("wrong numeric value");
            }
        return LiteralType::Decimal;
    }
}
