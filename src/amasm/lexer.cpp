#include "lexer.hpp"

//std
#include <stdexcept>

//alone
#include "general.hpp"

namespace alone::amasm {
	token_array_t Lexer::tokenize(const std::string& code) {
		token_array_t result;
		std::string temp;

		for (size_t i = 0, j = 0; i != code.size(); ++i) {
			if (is_alpha_numeric(code[i])) {
				temp += code[i];
			} else if (!temp.empty()) {
				result.emplace_back(temp);
				temp.clear();
			}

			if (singular_tokens.contains(code[i]))
				result.emplace_back(code[i]);
		}

		return result;
	}
}
