#include "literals.hpp"

//std
#include <iostream>

//shared
#include "shared/general_functions.hpp"

#define THROW_EXCEPTION(MESSAGE) std::cerr << __FUNCTION__ << (MESSAGE) << '\n'

namespace amasm::shared {
    LiteralType check_literal_type(const std::string& str) {
        //check whether it is word
        if (is_alpha(str.front())) {
            for (size_t i = 1; i < str.size(); i++)
                if (!is_alpha_numeric(str[i])) {
                    THROW_EXCEPTION("wrong word value");
                    return LiteralType::None;
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
                    THROW_EXCEPTION("wrong floating value");
                    return LiteralType::None;
                }
            for (i++; i < str.size(); i++)
                if (!is_numeric(str[i])) {
                    THROW_EXCEPTION("wrong floating value after '.'");
                    return LiteralType::None;
                }
            return LiteralType::Float;
        }

        //check whether it is binary number
        if (str.starts_with("0b")) {
            for (size_t i = 2; i < str.size(); i++)
                if (!is_binary(str[i])) {
                    THROW_EXCEPTION("wrong binary value");
                    return LiteralType::None;
                }
            return LiteralType::Binary;
        }

        //check whether it is hexadecimal number
        if (str.starts_with("0x")) {
            for (size_t i = 2; i < str.size(); i++)
                if (!is_hexadecimal(str[i])) {
                    THROW_EXCEPTION("wrong hex value");
                    return LiteralType::None;
                }
            return LiteralType::Hexadecimal;
        }

        //check whether it is integer number
        for (char i : str)
            if (!is_numeric(i)) {
                THROW_EXCEPTION("wrong numeric value");
                return LiteralType::None;
            }
        return LiteralType::Decimal;
    }
}
