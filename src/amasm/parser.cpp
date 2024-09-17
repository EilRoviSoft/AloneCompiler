#include "parser.hpp"

//std
#include <memory>
#include <stdexcept>
#include <stack>

//alone
#include "general.hpp"

namespace alone::amasm {
	bool Parser::init() {
		_init_basic_rules();

		return true;
	}

	void Parser::_init_basic_rules() {
		rules.emplace("optional", make_rule(parse_rule_flag::optional));
		rules.emplace("or", make_rule(parse_rule_flag::or_flag));
		rules.emplace("lparen", make_rule(token_type::lparen));
		rules.emplace("rparen", make_rule(token_type::rparen));
		rules.emplace("lbracket", make_rule(token_type::lbracket));
		rules.emplace("rbracket", make_rule(token_type::rbracket));
		rules.emplace("lbrace", make_rule(token_type::lbrace));
		rules.emplace("rbrace", make_rule(token_type::rbrace));
		rules.emplace("dot", make_rule(token_type::dot));
		rules.emplace("comma", make_rule(token_type::comma));
		rules.emplace("colon", make_rule(token_type::colon));
		rules.emplace("semicolon", make_rule(token_type::semicolon));
		rules.emplace("at", make_rule(token_type::at));
		rules.emplace("dollar", make_rule(token_type::dollar));
		rules.emplace("percent", make_rule(token_type::percent));
		rules.emplace("quote", make_rule(token_type::quote));
		rules.emplace("plus", make_rule(token_type::plus));
		rules.emplace("minus", make_rule(token_type::minus));
		rules.emplace("star", make_rule(token_type::star));
		rules.emplace("slash", make_rule(token_type::slash));
		rules.emplace("identifier", make_rule(token_type::identifier));
		rules.emplace("number", make_rule(token_type::number));

		rules.emplace("address_with_displacement", make_rule(std::initializer_list<std::string> {
			"lbracket", "percent", "identifier", "or", "plus", "minus", "rbracket"
		}));
	}

	bool
	Parser::_match_rules(const std::vector<std::string>& what, const std::vector<token_t>& tokens, size_t start_idx) {
		bool result = true;
		std::vector<parse_rule_ptr> rule_container;
		std::stack<token_t> stack;
		size_t di = 0, dj = 0;
		parse_rule_flag flag = parse_rule_flag::none;
		std::function<bool(size_t i, size_t j)> compare = [&rule_container, &tokens](size_t i, size_t j) {
			bool result;
			const auto& rule = rule_container[i];
			const auto& token = tokens[j];

			if (rule->type == parse_rule_type::singular_token) {
				result = rule->get_token_type() == token.type;
			} else if (rule->type == parse_rule_type::literal) {
				result = rule->get_literal() == token.literal;
			} else
				throw std::runtime_error("parser.cpp: idk how you got here...");

			return result;
		};

		rule_container.reserve(what.size());
		for (const auto& it: what)
			rule_container.push_back(rules[it]);

		//replace all complex rules
		for (size_t i = 0; i != rule_container.size(); ++i) {
			if (rule_container[i]->type == parse_rule_type::sequence) {
				auto seq = rule_container[i]->get_seq();
				rule_container.insert(rule_container.begin() + i, seq.begin(), seq.end());
				rule_container.erase(rule_container.begin() + i + seq.size());
			}
		}

		//directly matching
		for (size_t i = 0, j = start_idx; i != rule_container.size() && result; i += di, j += dj, di = 0, dj = 0) {
			if (rule_container[i]->type == parse_rule_type::flag) {
				flag = rule_container[i]->get_parse_rule_flag();
				di = 1;
				continue;
			}

			switch (flag) {
			case parse_rule_flag::none:
				result = compare(i, j);
				di = dj = 1;
				break;
			case parse_rule_flag::optional:
				di = dj = compare(i, j);
				break;
			case parse_rule_flag::or_flag:
				result = compare(i, j) || compare(i + 1, j + 1);
				di = dj = 2;
				break;
			}

			flag = parse_rule_flag::none;
		}

		return result;
	}
}
