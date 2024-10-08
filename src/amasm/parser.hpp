#pragma once

//std
#include <string>

//alone
#include "general.hpp"

//alone::amasm
#include "info/include.hpp"

namespace alone::amasm {
	class Parser {
	public:
		//TODO: make tree-based parsing
		static byte_array_t parse(const token_array_t& tokens);

		static bool match_rules(const std::string& rule_name, const token_array_t& tokens, size_t start_idx);
	};
}
