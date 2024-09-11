#pragma once

//std
#include <unordered_set>

//alone::amasm
#include "info.hpp"

namespace alone::amasm {
	class Lexer {
	public:
		static token_array_t tokenize(const std::string& code);

	private:
		static const std::unordered_set<char> _singular_tokens;
	};
}
