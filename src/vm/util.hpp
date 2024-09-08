#pragma once

//std
#include <tuple>

//alone::general
#include "general.hpp"

//alone::vm::info
#include "vm/info.hpp"

namespace alone::vm::util {
	inline std::tuple<info::memory_type, address_t> decompose(size_t address) {
		info::memory_type mem_type = info::memory_type(address & (gen_mask(63)) + (address & gen_mask(64)) * 2);
		address_t actual_address = address & ~(gen_mask(63) + gen_mask(64));
		return { mem_type, actual_address };
	}
}
