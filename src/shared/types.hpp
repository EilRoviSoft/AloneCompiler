#pragma once

//std
#include <bitset>
#include <cstddef>
#include <vector>

#define SYSTEM_MIN_TYPE std::byte
#define SYSTEM_MW_TYPE uint64_t
#define SYSTEM_INST_TYPE uint32_t
#define SYSTEM_BIT_DEPTH 64

namespace alone::shared {
	using data_sequence_t = std::vector<SYSTEM_MIN_TYPE>;
	using machine_word_t = SYSTEM_MW_TYPE;
	using address_t = SYSTEM_MW_TYPE;
	using inst_code_t = SYSTEM_INST_TYPE;
	using flags_t = std::bitset<SYSTEM_BIT_DEPTH>;

	constexpr size_t machine_word_size = sizeof(machine_word_t);
	constexpr size_t address_size = sizeof(address_t);
	constexpr size_t inst_size = sizeof(inst_code_t);
}
