#pragma once

//std
#include <string>

//alone
#include "general.hpp"

//alone::amasm::info
#include "amasm/info.hpp"

namespace alone::amasm::util {
	inline void sync_identifiers() {
		for(const auto& it : info::data_types)
			info::identifiers.try_emplace(it->literal, info::data_type);
		for(const auto& it : info::instructions)
			info::identifiers.try_emplace(it, info::instruction);
	}
}
