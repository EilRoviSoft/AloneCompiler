#pragma once

//std
#include <tuple>

//alone::general
#include "general.hpp"

//alone::vm::info
#include "vm/info.hpp"

namespace alone::vm {
	inline std::tuple<address_t, memory_type> decompose_address(address_t address) {
		return {
			(address & ~gen_mask(0, 1)) >> 2,
			memory_type(address & gen_mask(0, 1))
		};
	}
	inline std::tuple<inst_code_t, std::array<argument_type, 4>> decompose_instruction(inst_code_t instruction) {
		std::array<argument_type, 4> args;
		for (size_t i = 0; i != 4; ++i)
			args[i] = (argument_type) ((instruction & gen_mask(i * 2, 1 + i * 2)) >> i * 2);
		return {(instruction & ~gen_mask(0, 7)) >> 8,args };
	}
}
