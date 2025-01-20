#pragma once

//std
#include <queue>
#include <string>
#include <vector>

//compiler
#include "compiler/datatypes.hpp"
#include "compiler/instructions.hpp"
#include "compiler/variables.hpp"

namespace amasm::compiler {
    struct func_info {
        std::string name;
        datatype_ptr return_type;
        std::vector<datatype_ptr> args;
        VariablesCollection variables;
        std::vector<inst_decl> lines;
    };
    using funcs_queue = std::queue<func_info>;

    std::string generate_func_full_name(const func_info& func);
}
