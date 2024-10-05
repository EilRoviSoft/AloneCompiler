#include "parser.hpp"

//std
#include <memory>
#include <stdexcept>
#include <stack>

namespace alone::amasm {
	//sizes is more than 1, if it is variant
	struct flag_stack_cell {
		parse_rule_flag type = parse_rule_flag::none;
		size_t active_id = 0;
		std::vector<limit_t<size_t>> counters;
		std::vector<parse_rule_ptr> remembered;

		limit_t<size_t>& active() { return counters[counters.size() - active_id - 1]; }
		const limit_t<size_t>& active() const { return counters[counters.size() - active_id - 1]; }
	};

	code_t Parser::parse(const token_array_t& tokens) {
		for (size_t i = 0; i != tokens.size(); ++i) {

		}

		return {};
	}

	//TODO: WIP
	bool Parser::match_rules(const std::vector<std::string>& guide, const token_array_t& tokens, size_t start_idx) {
		std::vector<parse_rule_ptr> temp;

		temp.reserve(guide.size());
		for (const auto& str : guide) {
			parse_rule_ptr on_push;
			if (check_type(str) == literal_type::word)
				on_push = rules_collection[str];
			else
				on_push = make_rule(std::stoull(str, nullptr, 10));
			temp.push_back(on_push);
		}

		return match_rules(temp, tokens, start_idx);
	}
	bool Parser::match_rules(const std::string& guide, const token_array_t& tokens, size_t start_idx) {
		parse_rule_ptr temp;
		if (check_type(guide) == literal_type::word)
			temp = rules_collection[guide];
		else
			temp = make_rule(std::stoull(guide, nullptr, 10));

		return match_rules(std::vector(1, temp), tokens, start_idx);
	}
	bool Parser::match_rules(const parse_rule_ptr& guide, const token_array_t& tokens, size_t start_idx) {
		return match_rules(std::vector(1, guide), tokens, start_idx);
	}
	bool Parser::match_rules(const std::vector<parse_rule_ptr>& guide, const token_array_t& tokens, size_t start_idx) {
		bool result = true, temp;
		ptrdiff_t di = 0;
		std::stack<parse_rule_ptr> main;
		std::stack<flag_stack_cell> flags;
		std::function check_simple_rule = [](const parse_rule_ptr& rule, const token_t& token) {
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
		};

		flags.push(flag_stack_cell {});
		for (const auto& it : guide | std::views::reverse)
			main.push(it);

		for (size_t i = start_idx; result && !main.empty(); i += di, di = 0) {
			if (main.top()->type == parse_rule_type::flag) {
				flag_stack_cell on_push;

				on_push.type = main.top()->get_flag();
				main.pop();
				on_push.counters.reserve(main.top()->get_number());
				main.pop();
				flags.push(std::move(on_push));
				continue;
			}

			auto& current_flag = flags.top();
			if (current_flag.counters.size() < current_flag.counters.capacity())
				current_flag.counters.emplace_back(0, parse_rule_t::get_length(main.top()));

			if (main.top()->type == parse_rule_type::sequence) {
				auto seq = main.top()->get_sequence();
				main.pop();
				for (const auto& it : seq | std::views::reverse)
					main.push(it);
				continue;
			}

			switch (current_flag.type) {
			case parse_rule_flag::none:
				result = check_simple_rule(main.top(), tokens[i]);
				di = 1;
				main.pop();
				break;
			case parse_rule_flag::optional:
				if (current_flag.active().cur == current_flag.active().max) {
					flags.pop();
				} else if (check_simple_rule(main.top(), tokens[i])) {
					++current_flag.active().cur;
					di = 1;
					current_flag.remembered.push_back(main.top());
					main.pop();
				} else {
					di = -current_flag.active().cur;
					flags.pop();
				}
				break;
			case parse_rule_flag::variant:
				if (current_flag.active().cur == current_flag.active().max) {
					flags.pop();
				} else if (check_simple_rule(main.top(), tokens[i])) {
					++current_flag.active().cur;
					di = 1;
					main.pop();
				} else if (current_flag.active_id < current_flag.counters.size()) {
					di = -current_flag.active().cur;
					++current_flag.active_id;
					main.pop();
				} else
					result = false;
				break;
			case parse_rule_flag::skip_until_next_token:
				if (check_simple_rule(main.top(), tokens[i + 1]))
					flags.pop();
				di = 1;
				break;
			default:
				throw std::runtime_error("parser.cpp: Wrong flag type");
			}
		}

		return result;
	}
}
