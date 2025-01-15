#pragma once

//std
#include <queue>
#include <string>
#include <vector>

//amasm
#include "amasm/info/datatypes.hpp"
#include "amasm/info/instructions.hpp"
#include "amasm/info/variables.hpp"

namespace alone::amasm {
    struct func_info_t {
        std::string name;
        datatype_ptr return_type;
        std::vector<datatype_ptr> args;
        Variables variables;
        std::vector<inst_decl_t> lines;
    };
    using composed_funcs_t = std::queue<func_info_t>;

    std::string generate_func_full_name(const func_info_t& func);
}
