#pragma once

//std
#include <bitset>
#include <cstddef>
#include <vector>

namespace alone::shared {
	using byte_sequence_t = std::vector<std::byte>;
	using machine_word_t = uint64_t;
	using address_t = uint64_t;
	using inst_code_t = uint32_t;
	using flags_t = std::bitset<64>;
}
