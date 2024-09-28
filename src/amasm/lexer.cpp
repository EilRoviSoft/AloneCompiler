#include "lexer.hpp"

//std
#include <stdexcept>

//alone
#include "general.hpp"

namespace alone::amasm {
	token_array_t Lexer::tokenize(const std::string& code) {
		token_array_t result;
		std::string temp;

//		bool flag = false;
		for (size_t i = 0, j = 0; i != code.size(); ++i) {
			if (is_alpha_numeric(code[i])) {
				temp += code[i];
//				flag = true;
			} else if (!temp.empty()) {
				result.emplace_back(temp);
				temp.clear();
			}

			if (/*flag || */_singular_tokens.contains(code[i])) {
				result.emplace_back(code[i]);
//				flag = false;
			}
		}

		return result;
	}

	const std::unordered_set<char> Lexer::_singular_tokens = {
		'(', ')', '[', ']', '{', '}',
		'.', ',', ':', ';',
		'@', '$', '%', '\"',
		'+', '-', '*', '/'
	};
}
