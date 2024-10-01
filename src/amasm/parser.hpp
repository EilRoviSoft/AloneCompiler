#pragma once

//std
#include <fstream>
#include <string>
#include <tuple>

//alone::amasm
#include "info.hpp"
#include "general.hpp"

namespace alone::amasm {
	class Parser {
	public:
		/*static bool init();*/

		static bool match_rules(const std::vector<std::string>& guide,
		                        const std::vector<token_t>& tokens,
		                        size_t start_idx);

	private:
		static bool _check_simple_rule(const parse_rule_ptr& rule, const token_t& token);
	};
}
