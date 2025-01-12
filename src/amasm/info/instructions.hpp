#pragma once

//std
#include <list>
#include <string>

//shared
#include "shared/types.hpp"

//amasm
#include "amasm/info/arguments.hpp"

namespace alone::amasm {
    struct inst_info_t {
        std::string name;
        shared::inst_code_t code;
        size_t min_args;
        size_t max_args;
        size_t bid_depth;
    };
    struct inst_decl_t {
        std::string name;
        std::vector<argument_t> args;
    };

    std::list<inst_info_t> generate_isa_info();
}
