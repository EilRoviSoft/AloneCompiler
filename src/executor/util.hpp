#pragma once

//std
#include <tuple>

//library
#include "library/types.hpp"

//executor
#include "executor/constants.hpp"

namespace amasm::executor {
    std::tuple<lib::address, MemoryType> decompose_address(lib::address address);
    std::tuple<lib::inst_code, lib::args_data> decompose_instruction(lib::inst_code inst);
}
