#pragma once

//std
#include <queue>
#include <string>
#include <vector>

//compiler
#include "compiler/info/datatypes.hpp"
#include "compiler/info/instructions.hpp"
#include "compiler/info/variables.hpp"

namespace amasm::compiler {
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
