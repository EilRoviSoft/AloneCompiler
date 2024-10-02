#include "parser.hpp"

//std
#include <memory>
#include <stdexcept>
#include <stack>

namespace alone::amasm {
	//sizes is more than 1, if it is variant
	struct flag_stack_cell {
		parse_rule_flag flag;
		std::vector<size_t> sizes;

		flag_stack_cell(parse_rule_flag f, size_t ns) :
			flag(f) {
			sizes.reserve(ns);
		}
	};

	//TODO: WIP
	bool Parser::match_rules(const std::vector<std::string>& guide, const std::vector<token_t>& tokens, size_t start_idx) {
		bool result = true;
		std::stack<parse_rule_ptr> main;
		std::stack<flag_stack_cell> flags;
		std::function pop_from_stack = [&main]() {
			parse_rule_ptr result = main.top();
			main.pop();
			return result;
		};

		for (const auto& str : guide | std::views::reverse) {
			parse_rule_ptr on_push;
			if (check_type(str) == literal_type::word)
				on_push = rules_collection[str];
			else
				on_push = make_rule(std::stoull(str, nullptr, 10));
			main.push(on_push);
		}

		for (size_t i = start_idx, di = 0; result && !main.empty(); i += di, di = 0) {
			if (main.top()->type == parse_rule_type::flag) {
				auto arg0 = pop_from_stack();
				auto arg1 = pop_from_stack();
				flag_stack_cell on_push(arg0->get_flag(), arg1->get_number());
				flags.push(std::move(on_push));
				continue;
			}

			auto elem = pop_from_stack();
			/*switch (flags.top().flag) {
			case parse_rule_flag::variant:
			}*/

			if (main.top()->type == parse_rule_type::sequence) {
				auto seq = elem->get_sequence();
				for (const auto& it : seq | std::views::reverse)
					main.push(it);
			} else {
				result &= _check_simple_rule(elem, tokens[i]);
				di = 1;
			}
		}

		return result;
	}
	bool Parser::_check_simple_rule(const parse_rule_ptr& rule, const token_t& token) {
		bool result;
		switch (rule->type) {
		case parse_rule_type::singular_token:
			result = rule->get_token() == token.type;
			break;
		case parse_rule_type::literal:
			result = rule->get_literal() == token.literal;
			break;
		case parse_rule_type::number:
			result = rule->get_number() == std::stoull(token.literal);
			break;
		default:
			throw std::runtime_error("parser.cpp: Wrong parse_rule_type");
		}
		return result;
	}
}
