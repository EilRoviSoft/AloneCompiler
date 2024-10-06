#pragma once

//std
#include <string>

//alone
#include "general.hpp"

//alone::amasm
#include "info/include.hpp"

namespace alone::amasm {
	inline std::vector<token_t> convert(const std::vector<std::string>& raw) {
		std::vector<token_t> result;

		result.reserve(raw.size());
		for (const auto& it : raw) {
			if (it.size() == 1)
				result.emplace_back(it.front());
			else
				result.emplace_back(it);
		}

		return result;
	}
}
