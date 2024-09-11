#include "lexer.hpp"

//std
#include <exception>

//alone
#include "general.hpp"

namespace alone::amasm {
	token_array_t Lexer::tokenize(const std::string& code) {
		token_array_t result;
		std::string temp;

		for (size_t i = 0, j = 0; i != code.size(); ++i) {
			if (_singular_tokens.contains(code[i])) {
				result.emplace_back((token_type) code[i], code[i]);
				continue;
			}

			if (is_alpha_numeric(code[i])) {
				temp += code[i];
				continue;
			}

			if (!is_whitespace(code[i]) || temp.empty())
				continue;

			if (is_alpha(temp[0]) && check_on_property(temp.substr(1), &is_alpha_numeric)) {
				result.emplace_back(token_type::identifier, temp);
			} else if (check_on_property(temp, &is_hexadecimal)) {
				result.emplace_back(token_type::decimal, temp);
			} else
				throw(std::exception("lexer.cpp, 30: Wrong input"));

			temp.clear();
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
