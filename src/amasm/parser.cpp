#include "parser.hpp"

//std
#include <memory>
#include <stdexcept>
#include <stack>

namespace alone::amasm {
	//sizes is more than 1, if it is variant
	struct flag_stack_cell {
		parse_rule_flag f;
		std::vector<size_t> s;

		flag_stack_cell(parse_rule_flag flag, size_t size) :
			f(flag) {
			s.reserve(size);
		}
	};

	//TODO: WIP
	bool Parser::match_rules(const std::vector<std::string>& guide, const std::vector<token_t>& tokens, size_t start_idx) {
		bool result = true;
		Stack<parse_rule_ptr> main_stack;
		Stack<flag_stack_cell> flags_stack;

		for (const auto& str: guide | std::views::reverse) {
			parse_rule_ptr on_push;
			if (check_type(str) == literal_type::word)
				on_push = rules_collection[str];
			else
				on_push = make_rule(std::stoull(str, nullptr, 10));
			main_stack.push(on_push);
		}

		for (size_t i = start_idx, di = 0; result && !main_stack.is_empty(); i += di, di = 0) {
			if (main_stack.top()->type == parse_rule_type::flag) {
				auto arg0 = main_stack.pop();
				auto arg1 = main_stack.pop();
				flag_stack_cell on_push(arg0->get_flag(), arg1->get_number());
				for (size_t j = 0; j != on_push.s.capacity(); ++j)
					on_push.s.push_back(parse_rule_t::get_length(main_stack.get(j)));
				flags_stack.push(std::move(on_push));
				continue;
			} else if (main_stack.top()->type == parse_rule_type::sequence) {
				auto seq = main_stack.pop()->get_sequence();
				for (const auto& it: seq | std::views::reverse)
					main_stack.push(it);
			} else {
				result &= _check_simple_rule(main_stack.pop(), tokens[i]);
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
