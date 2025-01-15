#pragma once

//std
#include <cstdint>
#include <string>

namespace alone::shared {
    enum class Literals : uint8_t {
        None = 0,
        Word,
        Binary,
        Decimal,
        Hexadecimal,
        Float
    };

    Literals check_literal_type(const std::string& str);
}
