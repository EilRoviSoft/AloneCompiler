#pragma once

//std
#include <string>

//shared

namespace alone::amasm {
    enum class argument_type : uint8_t {
        empty = 0,                  // none
        direct,                     // idk how to define it now
        indirect,                   // name + value
        indirect_with_displacement, // name + value
        immediate,                  // value
        jump_address                // value
    };

    struct argument_t {
        argument_type type;
        std::string name;
        ptrdiff_t value;
    };
}
