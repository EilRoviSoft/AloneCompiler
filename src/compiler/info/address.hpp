#pragma once

//std
#include <string>

//library
#include "library/types.hpp"

namespace amasm::compiler {
    struct address_info {
        std::string name;
        size_t abs_value = 0;
        ptrdiff_t sign_value = 0;
        AddressType type = AddressType::Empty;
    };
}
