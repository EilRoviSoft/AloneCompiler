#include "parser.hpp"

//std
#include <memory>
#include <stack>
#include <stdexcept>

//alone
#include "error_codes.hpp"

#define WRONG_MATCH -1

namespace alone::amasm {
	struct scope_info_t {
		//std::vector<instruction_t> lines;
		//std::vector<variable_t> vars;
	};

	struct struct_info_t {
		std::string name;
		std::vector<pole_t> poles;
	};
	struct func_info_t {
		std::string name, full_name;
		data_type_ptr return_type;
		std::vector<data_type_ptr> args;
		scope_info_t scope;
	};

	using parse_variant_t = std::variant<struct_info_t, func_info_t, scope_info_t>;

	const std::unordered_set<std::string> surrounded_by_braces = { "struct_definition", "function_definition" };

	code_t Parser::parse(const token_array_t& tokens) {
		ptrdiff_t l;
		std::stack<std::pair<std::string, parse_variant_t>> stack;
		for (size_t i = 0, di = 0; i != tokens.size(); i += di, di = 0) {
			if (l = match_rules("struct_definition", tokens, i); l != WRONG_MATCH) {
				struct_info_t on_insert;
				on_insert.name = tokens[i + 1].literal;
				stack.emplace("struct_definition", std::move(on_insert));
				di = 3;
			} else if (l = match_rules("pole_definition", tokens, i); l != WRONG_MATCH) {
				if (stack.top().first != "struct_definition")
					throw AMASM_PARSER_WRONG_POLE_DEFINITION;
				auto& v = std::get<struct_info_t>(stack.top().second);
				v.poles.emplace_back(tokens[i + 2].literal,
					data_types[tokens[i + 4].literal],
					l == 10 ? 0 : std::stoull(tokens[i + 10].literal)
				);
				di = l;
			} else if (l = match_rules("function_definition", tokens, i); l != WRONG_MATCH) {
				func_info_t on_insert = { .name = tokens[i + 2].literal };
				//stack.emplace("function_definition");
			} else if (tokens[i].type == token_type::rbrace) { //closing body of struct/scope
				if (surrounded_by_braces.contains(stack.top().first)) {
					auto& v = std::get<struct_info_t>(stack.top().second);
					data_types.insert(make_data_type(v.name, v.poles));
					stack.pop();
					di = 1;
				} else
					throw AMASM_PARSER_WRONG_BRACE_PLACEMENT;
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

	ptrdiff_t Parser::match_rules(
		const std::vector<std::string>& guide, const token_array_t& tokens, size_t start_idx
	) {
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
	ptrdiff_t Parser::match_rules(const parse_array_t& guide, const token_array_t& tokens, size_t start_idx) {
		bool cond_flag = true;
		ptrdiff_t delta = 0, result = 0;
		std::stack<parse_rule_ptr> main;
		std::stack<flag_stack_cell_t> flags;

		flags.push(flag_stack_cell_t {});
		for (const auto& it : guide | std::views::reverse)
			main.push(it);

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
			if (current_flag.args_counter.n < current_flag.args_counter.max && current_flag.curr_counter.max == 0 &&
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
				cond_flag = _match_simple_rule(main.top(), tokens[i]);
				delta = 1;
				main.pop();
				break;
			case parse_flag_type::optional:
				if (current_flag.curr_counter.n == current_flag.curr_counter.max) {
					flags.pop();
				} else if (_match_simple_rule(main.top(), tokens[i])) {
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
				} else if (_match_simple_rule(main.top(), tokens[i])) {
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
				if (_match_simple_rule(rules.at("}"), tokens[i + 1]))
					flags.pop();
				delta = 1;
				break;
			case parse_flag_type::skip_args:
				if (current_flag.args_counter.n % 2) {
					if (_match_simple_rule(rules.at(","), tokens[i + 1]))
						++current_flag.args_counter.n;
					else if (_match_simple_rule(rules.at(")"), tokens[i + 1]))
						flags.pop();
					else
						throw AMASM_PARSER_WRONG_FUNC_ARGS_DEFINITION;
					delta = 1;
				} else if (_match_simple_rule(rules.at("data_type"), tokens[i + 1])) {
					++current_flag.args_counter.n;
					delta = 1;
				} else
					throw AMASM_PARSER_WRONG_FUNC_ARGS_DEFINITION;
				break;
			default:
				throw AMASM_PARSER_WRONG_FLAG_TYPE;
			}
		}

		return cond_flag ? result : -1;
	}

	bool Parser::_match_simple_rule(const parse_rule_ptr& rule, const token_t& token) {
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
			throw AMASM_PARSER_WRONG_PARSE_RULE_TYPE;
		}
		return result;
	}
}
