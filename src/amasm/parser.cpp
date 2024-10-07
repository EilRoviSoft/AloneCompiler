#include "parser.hpp"

//std
#include <memory>
#include <stdexcept>
#include <stack>

#define WRONG_MATCH -1

namespace alone::amasm {
	struct struct_info_t {
		std::string name;
		std::vector<pole_t> poles;
	};

	using parse_variant_t = std::variant<struct_info_t>;

	std::unordered_set<std::string> surrounded_by_braces = {
		"struct_definition"
	};

	code_t Parser::parse(const token_array_t& tokens) {
		ptrdiff_t l;
		std::stack<std::pair<std::string, parse_variant_t>> stack;
		for (size_t i = 0, di = 0; i != tokens.size(); i += di, di = 0) {
			if (l = match_rules("struct_definition", tokens, i); l != WRONG_MATCH) {
				stack.emplace("struct_definition", struct_info_t { .name = tokens[i + 1].literal });
				di = 3;
			} else if (l = match_rules("pole_definition", tokens, i); l != WRONG_MATCH) {
				if (stack.top().first != "struct_definition")
					throw std::runtime_error("parser.cpp: pole is defined outside of struct.");
				auto& v = std::get<struct_info_t>(stack.top().second);
				v.poles.emplace_back(
					tokens[i + 2].literal,
					data_types[tokens[i + 4].literal],
					l == 10 ? 0 : std::stoull(tokens[i + 10].literal)
				);
				di = l;
			} else if (tokens[i].type == token_type::rbrace) { //closing body of struct/func/etc
				if (stack.top().first == "struct_definition") {
					auto& v = std::get<struct_info_t>(stack.top().second);
					data_types.insert(make_data_type(v.name, v.poles));
					stack.pop();
					di = 1;
				} else
					throw std::runtime_error("parser.cpp: can't close braces.");
			}
		}

		return {};
	}

	//sizes is more than 1, if it is variant
	struct flag_stack_cell_t {
		parse_flag_type type = parse_flag_type::none;
		limit_t<size_t> args_counter = { 0, 0 }, curr_counter = { 0, 0 };
		std::vector<parse_rule_ptr> remembered;
	};

	ptrdiff_t Parser::match_rules(const std::vector<std::string>& guide, const token_array_t& tokens, size_t start_idx) {
		std::vector<parse_rule_ptr> temp;

		temp.reserve(guide.size());
		for (const auto& str : guide) {
			parse_rule_ptr on_push;
			if (check_type(str) == literal_type::word)
				on_push = rules.at(str);
			else
				on_push = make_rule(std::stoull(str, nullptr, 10));
			temp.push_back(on_push);
		}

		return match_rules(temp, tokens, start_idx);
	}
	ptrdiff_t Parser::match_rules(const std::string& guide, const token_array_t& tokens, size_t start_idx) {
		parse_rule_ptr temp;
		if (check_type(guide) == literal_type::word)
			temp = rules[guide];
		else
			temp = make_rule(std::stoull(guide, nullptr, 10));

		return match_rules(std::vector(1, temp), tokens, start_idx);
	}
	ptrdiff_t Parser::match_rules(const parse_rule_ptr& guide, const token_array_t& tokens, size_t start_idx) {
		return match_rules(std::vector(1, guide), tokens, start_idx);
	}
	ptrdiff_t Parser::match_rules(const std::vector<parse_rule_ptr>& guide, const token_array_t& tokens, size_t start_idx) {
		bool cond_flag = true;
		ptrdiff_t delta = 0, result = 0;
		std::stack<parse_rule_ptr> main;
		std::stack<flag_stack_cell_t> flags;
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

		flags.push(flag_stack_cell_t {});
		for (const auto& it : guide | std::views::reverse) {
			main.push(it);
		}

		for (size_t i = start_idx; cond_flag && !main.empty(); i += delta, result += delta, delta = 0) {
			if (main.top()->type == parse_rule_type::flag) {
				flag_stack_cell_t on_push;

				on_push.type = main.top()->get_flag();
				main.pop();
				switch (on_push.type) {
				case parse_flag_type::optional:
				case parse_flag_type::variant:
					on_push.args_counter.max = main.top()->get_number();
					main.pop();
					break;
				default:
					break;
				}
				flags.push(std::move(on_push));
				continue;
			}

			auto& current_flag = flags.top();
			if (current_flag.args_counter.n < current_flag.args_counter.max &&
				current_flag.curr_counter.max == 0 &&
				current_flag.curr_counter.n == current_flag.curr_counter.max) {
				current_flag.curr_counter = { 0, parse_rule_t::get_length(main.top()) };
				++current_flag.args_counter.n;
			}

			if (main.top()->type == parse_rule_type::sequence) {
				auto seq = main.top()->get_sequence();
				main.pop();
				for (const auto& it : seq | std::views::reverse)
					main.push(it);
				continue;
			}

			switch (current_flag.type) {
			case parse_flag_type::none:
				cond_flag = check_simple_rule(main.top(), tokens[i]);
				delta = 1;
				main.pop();
				break;
			case parse_flag_type::optional:
				if (current_flag.curr_counter.n == current_flag.curr_counter.max) {
					flags.pop();
				} else if (check_simple_rule(main.top(), tokens[i])) {
					++current_flag.curr_counter.n;
					delta = 1;
					current_flag.remembered.push_back(main.top());
					main.pop();
				} else {
					for (size_t j = current_flag.curr_counter.n; j != current_flag.curr_counter.max; ++j)
						main.pop();
					for (size_t j = current_flag.args_counter.n; j != current_flag.args_counter.max; ++j)
						main.pop();
					delta = -current_flag.curr_counter.n;
					flags.pop();
				}
				break;
			case parse_flag_type::variant:
				if (current_flag.curr_counter.n == current_flag.curr_counter.max) {
					for (size_t j = current_flag.curr_counter.n; j != current_flag.curr_counter.max; ++j)
						main.pop();
					for (size_t j = current_flag.args_counter.n; j != current_flag.args_counter.max; ++j)
						main.pop();
					flags.pop();
				} else if (check_simple_rule(main.top(), tokens[i])) {
					++current_flag.curr_counter.n;
					delta = 1;
					main.pop();
				} else if (current_flag.args_counter.n < current_flag.args_counter.max) {
					delta = -current_flag.curr_counter.n;
					++current_flag.args_counter.n;
					main.pop();
				} else
					cond_flag = false;
				break;
			case parse_flag_type::skip_body:
				if (check_simple_rule(main.top(), tokens[i + 1]))
					flags.pop();
				delta = 1;
				break;
			case parse_flag_type::skip_args:

				break;
			default:
				throw std::runtime_error("parser.cpp: wrong flag type.");
			}
		}

		return cond_flag ? result : -1;
	}
}
