#pragma once

//std
#include <cstdint>
#include <string>

namespace amasm::lib {
    enum class LiteralType : uint8_t {
        None = 0,
        Word,
        Binary,
        Decimal,
        Hexadecimal,
        Float
    };

    LiteralType check_literal_type(const std::string& str);
}
