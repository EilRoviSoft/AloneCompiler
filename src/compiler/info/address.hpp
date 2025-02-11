#pragma once

//std
#include <string>

//library
#include "library/types.hpp"

namespace amasm::compiler {
    struct address_info {
        std::string name;
        ptrdiff_t value;
        AddressType type;
    };
}
