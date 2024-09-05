#pragma once
//alone::vm::info
#include "vm/info.hpp"

namespace alone::vm::util {
	constexpr size_t gen_mask(size_t pos);
	constexpr std::tuple<info::memory_type, address_t> decompose(size_t address);
}
