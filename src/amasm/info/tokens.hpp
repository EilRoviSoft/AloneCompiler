#pragma once

//std
#include <string>
#include <vector>

//alone::amasm::info
#include "enums.hpp"

namespace alone::amasm {
	struct token_t {
		token_type type;
		std::string literal;

		token_t();
		explicit token_t(char c);
		explicit token_t(std::string s);
	};

	using token_array_t = std::vector<token_t>;
}
