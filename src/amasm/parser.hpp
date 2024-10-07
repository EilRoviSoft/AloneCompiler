#pragma once

//std
#include <fstream>
#include <string>
#include <tuple>
#include <variant>

//alone
#include "general.hpp"

//alone::amasm
#include "info/include.hpp"

namespace alone::amasm {
	//TODO: match_rules return matched size
	class Parser {
	public:
		static code_t parse(const token_array_t& tokens);

		static ptrdiff_t match_rules(const std::vector<std::string>& guide,
		                             const token_array_t& tokens,
		                             size_t start_idx);
		static ptrdiff_t match_rules(const std::string& guide,
		                             const token_array_t& tokens,
		                             size_t start_idx);
		static ptrdiff_t match_rules(const parse_rule_ptr& guide,
		                             const token_array_t& tokens,
		                             size_t start_idx);
		static ptrdiff_t match_rules(const std::vector<parse_rule_ptr>& guide,
		                             const token_array_t& tokens,
		                             size_t start_idx);

	private:

	};
}
