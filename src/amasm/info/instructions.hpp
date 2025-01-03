#pragma once

//std
#include <list>
#include <string>

//shared
#include "shared/types.hpp"

namespace alone::amasm {
    struct inst_info_t {
        std::string name;
        shared::inst_code_t code;
        size_t min_args;
        size_t max_args;
        size_t bid_depth;
    };

    std::list<inst_info_t> generate_isa_info();
}
