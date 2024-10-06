#pragma once

//std
#include <fstream>
#include <string>
#include <tuple>

//alone
#include "general.hpp"

//alone::amasm
#include "info/include.hpp"

namespace alone::amasm {
	class Parser {
	public:
		static code_t parse(const token_array_t& tokens);

		/*static bool match_rules(const std::vector<std::string>& guide,
		                        const token_array_t& tokens,
		                        size_t start_idx);*/
		static bool match_rules(const std::string& guide,
		                        const token_array_t& tokens,
		                        size_t start_idx);
		/*static bool match_rules(const parse_rule_ptr& guide,
		                        const token_array_t& tokens,
		                        size_t start_idx);*/
		static bool match_rules(const std::vector<parse_rule_ptr>& guide,
		                        const token_array_t& tokens,
		                        size_t start_idx);
	};
}
