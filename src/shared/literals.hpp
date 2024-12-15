#pragma once

//std
#include <cstdint>
#include <string>

namespace alone::shared {
	enum class literal_type : uint8_t {
        none = 0,
        word,
        binary, 
        decimal, 
        hexadecimal, 
        floating
    };

    literal_type check_literal_type(const std::string& str);
}
