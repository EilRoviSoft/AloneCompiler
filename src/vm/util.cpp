#include "util.hpp"
//std
#include <tuple>

namespace alone::vm::util {
	//pos [1, sizeof(size_t)]
	constexpr size_t gen_mask(size_t pos) {
		return 1ull << (pos - 1);
	}
	//memory type and actual address
	std::tuple<info::memory_type, address_t> decompose(size_t address) {
		info::memory_type mem_type = info::memory_type(address & (gen_mask(63)) + (address & gen_mask(64)) * 2);
		address_t actual_address = address & ~(gen_mask(63) + gen_mask(64));
		return { mem_type, actual_address };
	}
}
