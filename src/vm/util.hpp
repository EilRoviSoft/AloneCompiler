#pragma once

//std
#include <tuple>

//alone::general
#include "general.hpp"

//alone::vm::info
#include "vm/info.hpp"

namespace alone::vm::util {
	inline std::tuple<size_t, info::memory_type> decompose_address(address_t address) {
		return {
			(address & ~gen_mask(0, 1)) >> 2,
			info::memory_type(address & gen_mask(0, 1))
		};
	}
	inline std::tuple<uint32_t, std::array<info::argument_type, 4>> decompose_instruction(inst_code_t instruction) {
		return {
			(instruction & ~gen_mask(0, 7)) >> 8,
			[instruction]() {
				std::array<info::argument_type, 4> args;
				for(size_t i = 0; i != 4; ++i)
					args[i] = (info::argument_type) ((instruction & gen_mask(i * 2, 1 + i * 2)) >> i * 2);
				return args;
			}()
		};
	}
}
