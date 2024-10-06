#include "lexer.hpp"

//std
#include <stdexcept>

//alone
#include "general.hpp"

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

			if (singular_tokens.contains(c)) result.emplace_back(c);
		}

		return result;
	}
	std::vector<parse_rule_ptr> Lexer::tokenize_rule_sequence(const std::string& seq) {
		parse_rule_t::sequence_rule_t result;
		std::string temp;
		bool flag = true;

		for (auto c : seq) {
			if (is_alpha_numeric(c)) {
				temp += c;
				flag = false;
			} else if (!temp.empty()) {
				auto type = check_type(temp);
				switch (type) {
				case literal_type::word:
					result.emplace_back(rules[temp]);
					break;
				default:
				}
				temp.clear();
			}

			if (flag && rules.contains(std::string(1, c))) result.emplace_back(rules[temp]);

			flag = true;
		}

		return result;
	}
}
