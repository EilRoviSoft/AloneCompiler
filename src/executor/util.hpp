#pragma once

//std
#include <tuple>

//shared
#include "shared/types.hpp"

//executor
#include "executor/constants.hpp"

namespace amasm::executor {
    std::tuple<shared::address, MemoryType> decompose_address(shared::address address);
    std::tuple<shared::inst_code, shared::args_data> decompose_instruction(shared::inst_code inst);
}
