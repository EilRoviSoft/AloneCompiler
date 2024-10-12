#include "parser.hpp"

//std
#include <memory>
#include <queue>
#include <stdexcept>
#include <variant>

//alone
#include "error_codes.hpp"

//alone::amasm
#include <iostream>

#include "info/extras.hpp"

namespace alone::amasm::inline parser_inlined {
	struct inst_call_t {
		std::string name;
		std::vector<argument_t> args;
	};

	struct scope_info_t {
		std::vector<inst_call_t> lines;
		std::vector<variable_t> vars;
	};

	struct func_info_t {
		std::string name;
		data_type_ptr return_type;
		std::vector<data_type_ptr> args;
		scope_info_t scope;
	};

	using parse_variant_t = std::variant<data_type_ptr, func_info_t>;
	using parse_queue_t = std::queue<std::pair<std::string, parse_variant_t>>;
	using context_t = std::tuple<const token_array_t&, parse_queue_t&, const size_t&, size_t&>;

	const std::unordered_set<std::string> surrounded_by_braces = { "struct_definition", "function_definition" };

	bool match_rule(const std::string& rule_name, const token_array_t& tokens, size_t start_idx) {
		const auto& pattern = rules.at(rule_name);
		bool result = true;

		for (size_t i = 0, diff = 0; result && i != pattern.size(); ++i)
			result = pattern[i + diff] == tokens[start_idx + i].type;

		return result;
	}
	std::tuple<ptrdiff_t, size_t> calc_offset(const data_type_ptr& type, const token_array_t& tokens, size_t start_idx) {
		data_type_ptr current_type = type;
		ptrdiff_t offset = 0;
		size_t delta = 0;

		for (size_t i = start_idx + 1, di = 0; tokens[i].type != token_type::semicolon &&
			 tokens[i].type == token_type::comma; i += 2, delta += 2) {
			auto it = std::ranges::find_if(current_type->poles, [&](const pole_t& pole) {
				return pole.name == tokens[i + 1].literal;
			});
			current_type = it->type;
			offset += it->offset;
		}

		return { offset, delta };
	}

	void start_parse_struct(context_t& ctx) {
		auto& [tokens, stack, i, di] = ctx;
		if (!match_rule("struct_definition", tokens, i))
			throw AMASM_PARSER_WRONG_STRUCT_DEFINITION;

		data_type_ptr on_push = make_data_type(tokens[i + 1].literal, 0);
		di = 3;
		stack.emplace("struct_definition", std::move(on_push));
	}
	void start_parse_function(context_t& ctx) {
		auto& [tokens, stack, i, di] = ctx;
		if (!match_rule("function_definition", tokens, i))
			throw AMASM_PARSER_WRONG_FUNC_DEFINITION;

		size_t j;
		func_info_t on_push;

		on_push.name = tokens[i + 2].literal;
		//arguments dispatching
		for (j = i + 4; tokens[j].type != token_type::rparen; ++j) {
			if ((j - i) % 2) {
				if (tokens[j].type != token_type::comma)
					throw AMASM_PARSER_WRONG_FUNC_DEFINITION;
			} else if (tokens[j].type == token_type::data_type) {
				on_push.args.push_back(data_types.at(tokens[j].literal));
			} else
				throw AMASM_PARSER_WRONG_FUNC_DEFINITION;
		}

		//return type dispatching
		if (tokens[j + 1].type == token_type::colon) {
			on_push.return_type = data_types.at(tokens[j + 2].literal);
			di = j - i + 4;
		} else {
			on_push.return_type = data_types.at("void");
			di = j - i + 2;
		}
		stack.emplace("function_definition", std::move(on_push));
	}

	void parse_variable(context_t& ctx) {
		auto& [tokens, stack, i, di] = ctx;
		if (stack.back().first == "struct_definition") {
			if (!match_rule("pole_definition", tokens, i))
				throw AMASM_PARSER_WRONG_POLE_DEFINITION;

			auto& ntype = std::get<data_type_ptr>(stack.back().second);
			bool has_own_offset = tokens[i + 5].type == token_type::comma;

			ntype->add_pole(tokens[i + 2].literal, data_types.at(tokens[i + 4].literal));
			di = has_own_offset ? 13 : 6;
		} else {}
	}
	void parse_instruction(context_t& ctx) {
		auto& [tokens, stack, i, di] = ctx;
		if (stack.back().first != "function_definition")
			throw AMASM_PARSER_WRONG_INST_DEFINITION_PLACE;

		size_t j, dj, args_n;
		auto& func = std::get<func_info_t>(stack.back().second);
		auto inst_info = instructions.at(tokens[i].literal);
		inst_call_t on_push;

		on_push.name = tokens[i].literal;
		for (j = i, dj = 0, args_n = 0; args_n != inst_info.max_args_count &&
			 tokens[j].type != token_type::semicolon; j += dj, dj = 0, ++args_n) {
			argument_t arg;

			if (tokens[j].type != token_type::inst_name && tokens[j].type != token_type::comma)
				throw AMASM_PARSER_WRONG_INST_ARGS_DEFINITION;

			if (match_rule("direct", tokens, j + 1)) {
				auto [var_offset, delta] = calc_offset(get_data_type(on_push.name), tokens, j + 3);
				arg = {
					var_offset ? argument_type::indirect_with_displacement : argument_type::direct,
					tokens[j + 2].literal,
					var_offset
				};
				dj = delta + 3;
			} else if (tokens[j + 1].type == token_type::number) {
				arg = {
					argument_type::immediate,
					"",
					std::stoll(tokens[j + 1].literal)
				};
				dj = 2;
			} else if (match_rule("indirect", tokens, j + 1)) {
				auto [var_offset, delta] = calc_offset(get_data_type(on_push.name), tokens, j + 4);
				arg = {
					argument_type::indirect_with_displacement,
					tokens[j + 3].literal,
					var_offset
				};

				if (tokens[j + delta + 4].type == token_type::plus) {
					arg.value += std::stoll(tokens[j + delta + 5].literal);
					delta += 2;
				} else if (tokens[j + delta + 4].type == token_type::minus) {
					arg.value -= std::stoll(tokens[j + delta + 5].literal);
					delta += 2;
				}
				dj = delta + 5;
			} else
				throw AMASM_PARSER_WRONG_INST_DEFINITION;

			on_push.args.push_back(arg);
		}

		func.scope.lines.emplace_back(std::move(on_push));
		di = j - i + 1 + (inst_info.max_args_count == 0);
	}

	void finish_parse_rbrace(context_t& ctx) {
		auto& [tokens, stack, i, di] = ctx;
		if (stack.back().first == "struct_definition") {
			auto ntype = std::get<data_type_ptr>(stack.back().second);
			stack.pop();
			data_types.emplace(ntype->name, ntype);
		}
		di = 1;
	}

	const std::unordered_map<token_type, std::function<void(context_t&)>> parse_rules = {
		{ token_type::kw_struct, start_parse_struct },
		{ token_type::kw_func, start_parse_function },
		{ token_type::kw_var, parse_variable },
		{ token_type::inst_name, parse_instruction },
		{ token_type::rbrace, finish_parse_rbrace }
	};
}

namespace alone::amasm {
	byte_array_t Parser::parse(const token_array_t& tokens) {
		size_t i, di;
		parse_queue_t queue;
		context_t ctx = { tokens, queue, i, di };

		for (i = 0, di = 0; i != tokens.size(); i += di, di = 0) {
			if (auto it = parse_rules.find(tokens[i].type); it != parse_rules.end())
				it->second(ctx);
			else
				throw AMASM_PARSER_TOKEN_DOESNT_EXIST;
		}

		return {};
	}
}
