#include "lexer.hpp"

//std
#include <stdexcept>

//alone
#include "general.hpp"
#include "error_codes.hpp"

namespace alone::amasm {
	token_array_t Lexer::tokenize_code(const std::string& code) {
		token_array_t result;
		std::string temp;

		for (auto c : code) {
			if (is_alpha_numeric(c)) {
				temp += c;
			} else if (!temp.empty()) {
				result.emplace_back(temp);
				temp.clear();
			}

			if (singular_tokens.contains(c))
				result.emplace_back(c);
		}

		return result;
	}
	std::vector<parse_rule_ptr> Lexer::tokenize_rule_sequence(const std::string& seq) {
		parse_rule_t::sequence_rule_t result;
		std::string temp;
		bool flag = true;

		for (char c : seq) {
			if (is_alpha_numeric(c)) {
				temp += c;
				flag = false;
			} else if (!temp.empty()) {
				switch (auto type = check_type(temp); type) {
				case literal_type::word:
					result.emplace_back(rules.at(temp));
					break;
				case literal_type::none:
					throw AMASM_LEXER_WRONG_LITERAL_TYPE;
				default:
					result.emplace_back(make_rule(temp));
					break;
				}
				temp.clear();
			}

			if (auto s = std::string(1, c); flag && rules.contains(s))
				result.push_back(rules.at(s));

			flag = true;
		}

		return result;
	}
}
