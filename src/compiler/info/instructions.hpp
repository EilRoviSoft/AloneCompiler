#pragma once

//std
#include <list>
#include <string>
#include <tuple>

//shared
#include "shared/types.hpp"

namespace amasm::compiler {
    struct inst_info {
        std::string name;
        shared::InstCode code;
        size_t min_args;
        size_t max_args;
        size_t bid_depth;
    };

    struct argument_info {
        ArgumentType type;
        std::string name;
        ptrdiff_t value;
    };

    struct inst_decl {
        std::string name;
        std::vector<argument_info> args;
    };

    std::list<inst_info> generate_isa_info();

    using composed_inst = std::tuple<size_t, inst_decl>;
}
