#pragma once
//std
#include <array>
#include <cstdint>
//alone::info
#include "info.hpp"

namespace alone::util {
	constexpr size_t gen_mask(size_t pos);
	constexpr std::tuple<info::memory_type, info::address_t> decompose(size_t address);
}
