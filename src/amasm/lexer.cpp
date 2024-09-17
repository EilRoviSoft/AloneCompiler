#include "lexer.hpp"

//std
#include <stdexcept>

//alone
#include "general.hpp"

namespace alone::amasm {
	token_array_t Lexer::tokenize(const std::string& code) {
		token_array_t result;
		std::string temp;
		auto gen_token = [](const std::string& s) {
			token_type result;
			if (is_alpha(s[0]) && check_on_property(s.substr(1), &is_alpha_numeric)) {
				result = token_type::identifier;
			} else if (check_on_property(s, &is_hexadecimal)) {
				result = token_type::number;
			} else
				throw std::runtime_error("lexer.cpp: Wrong input");
			return result;
		};

		bool flag = false;
		for (size_t i = 0, j = 0; i != code.size(); ++i) {
			if (is_alpha_numeric(code[i])) {
				temp += code[i];
				flag = true;
			} else if (!temp.empty()) {
				result.emplace_back(gen_token(temp), temp);
				temp.clear();
			}

			if (flag && _singular_tokens.contains(code[i])) {
				result.emplace_back((token_type) code[i], code[i]);
				flag = false;
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
