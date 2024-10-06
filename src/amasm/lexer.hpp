#pragma once

//std
#include <unordered_set>

//alone::amasm
#include "info/include.hpp"

namespace alone::amasm {
	class Lexer {
	public:
		static token_array_t tokenize_code(const std::string& code);
		static std::vector<parse_rule_ptr> tokenize_rule_sequence(const std::string& seq);
	};
}
