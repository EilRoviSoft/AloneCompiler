#include "literals.hpp"

//std
#include <iostream>

//shared
#include "shared/general_functions.hpp"

#define THROW_EXCEPTION(MESSAGE) std::cerr << __FUNCTION__ << (MESSAGE) << '\n'

namespace alone::shared {
    literal_type check_literal_type(const std::string& str) {
        //check whether it is word
        if (is_alpha(str.front())) {
            for (size_t i = 1; i < str.size(); i++)
                if (!is_alpha_numeric(str[i])) {
                    THROW_EXCEPTION("wrong word value");
                    return literal_type::none;
                }
            return literal_type::word;
        }

        //wtf is that?
        if (!is_numeric(str.front()))
            return literal_type::none;

        //check whether it is floating number
        if (str.contains('.')) {
            size_t i;
            for (i = 1; str[i] != '.'; i++)
                if (!is_numeric(str[i])) {
                    THROW_EXCEPTION("wrong floating value");
                    return literal_type::none;
                }
            for (i++; i < str.size(); i++)
                if (!is_numeric(str[i])) {
                    THROW_EXCEPTION("wrong floating value after '.'");
                    return literal_type::none;
                }
            return literal_type::floating;
        }

        //check whether it is binary number
        if (str.starts_with("0b")) {
            for (size_t i = 2; i < str.size(); i++)
                if (str[i] != '0' && str[i] != '1') {
                    THROW_EXCEPTION("wrong binary value");
                    return literal_type::none;
                }
            return literal_type::binary;
        }

        //check whether it is hexadecimal number
        if (str.starts_with("0x")) {
            for (size_t i = 2; i < str.size(); i++)
                if (!is_hexadecimal(str[i])) {
                    THROW_EXCEPTION("wrong hex value");
                    return literal_type::none;
                }
            return literal_type::hexadecimal;
        }

        //check whether it is integer number
        for (size_t i = 0; i < str.size(); i++)
            if (!is_numeric(str[i])) {
                THROW_EXCEPTION("wrong numeric value");
                return literal_type::none;
            }
        return literal_type::decimal;
    }
}
