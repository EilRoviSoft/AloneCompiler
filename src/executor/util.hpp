#pragma once

//std
#include <tuple>

//shared
#include "shared/types.hpp"

//executor
#include "executor/constants.hpp"

namespace amasm::executor {
	std::tuple<shared::Address, MemoryType> decompose_address(shared::Address address);
	std::tuple<shared::InstCode, shared::ArgumentsData> decompose_instruction(shared::InstCode inst);
}
