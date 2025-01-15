#pragma once

//std
#include <string>

//shared

namespace alone::amasm {
    enum class Arguments : uint8_t {
        Empty = 0,                // none
        Direct,                   // idk how to define it now
        IndirectWithDisplacement, // name + value
        Immediate,                // value
        JumpAddress               // value
    };

    struct argument_t {
        Arguments type;
        std::string name;
        ptrdiff_t value;
    };
}
