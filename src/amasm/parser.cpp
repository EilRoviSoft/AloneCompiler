#include "parser.hpp"

//std
#include <memory>
#include <queue>
#include <stdexcept>
#include <variant>

//alone
#include "error_codes.hpp"
#include "info/extras.hpp"
#include "instructions.hpp"

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

	struct parse_context_t {
		const token_array_t& tokens;
		const std::unordered_map<std::string, size_t>& labels;
		parse_queue_t& queue;
		const size_t& i;
		size_t& di;
	};
	struct translate_context_t {
		std::unordered_map<std::string, size_t>& labels;
		const parse_variant_t& info;
		size_t& i;
	};

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

	void start_parse_struct(parse_context_t& ctx) {
		if (!match_rule("struct_definition", ctx.tokens, ctx.i))
			throw AMASM_PARSER_WRONG_STRUCT_DEFINITION;

		data_type_ptr on_push = make_data_type(ctx.tokens[ctx.i + 1].literal, 0);
		ctx.di = 3;
		ctx.queue.emplace("struct_definition", std::move(on_push));
	}
	void start_parse_function(parse_context_t& ctx) {
		if (!match_rule("function_definition", ctx.tokens, ctx.i))
			throw AMASM_PARSER_WRONG_FUNC_DEFINITION;

		size_t j;
		func_info_t on_push;

		on_push.name = ctx.tokens[ctx.i + 2].literal;
		//arguments dispatching
		for (j = ctx.i + 4; ctx.tokens[j].type != token_type::rparen; ++j) {
			if ((j - ctx.i) % 2) {
				if (ctx.tokens[j].type != token_type::comma)
					throw AMASM_PARSER_WRONG_FUNC_DEFINITION;
			} else if (ctx.tokens[j].type == token_type::data_type) {
				on_push.args.push_back(data_types.at(ctx.tokens[j].literal));
			} else
				throw AMASM_PARSER_WRONG_FUNC_DEFINITION;
		}

		//return type dispatching
		if (ctx.tokens[j + 1].type == token_type::colon) {
			on_push.return_type = data_types.at(ctx.tokens[j + 2].literal);
			ctx.di = j - ctx.i + 4;
		} else {
			on_push.return_type = data_types.at("void");
			ctx.di = j - ctx.i + 2;
		}
		ctx.queue.emplace("function_definition", std::move(on_push));
	}

	void parse_variable(parse_context_t& ctx) {
		if (ctx.queue.back().first == "struct_definition") {
			if (!match_rule("pole_definition", ctx.tokens, ctx.i))
				throw AMASM_PARSER_WRONG_POLE_DEFINITION;

			auto& ntype = std::get<data_type_ptr>(ctx.queue.back().second);
			bool has_own_offset = ctx.tokens[ctx.i + 5].type == token_type::comma;

			ntype->add_pole(ctx.tokens[ctx.i + 2].literal, data_types.at(ctx.tokens[ctx.i + 4].literal));
			ctx.di = has_own_offset ? 13 : 6;
		} else {}
	}
	void parse_instruction(parse_context_t& ctx) {
		if (ctx.queue.back().first != "function_definition")
			throw AMASM_PARSER_WRONG_INST_DEFINITION_PLACE;

		size_t j, dj, args_n;
		auto& func = std::get<func_info_t>(ctx.queue.back().second);
		const auto& inst_info = instructions_by_name.at(ctx.tokens.at(ctx.i).literal);
		inst_call_t on_push;

		on_push.name = ctx.tokens[ctx.i].literal;
		for (j = ctx.i, dj = 0, args_n = 0; args_n != inst_info->max_args_count &&
			 ctx.tokens[j].type != token_type::semicolon; j += dj, dj = 0, ++args_n) {
			argument_t arg;

			if (ctx.tokens[j].type != token_type::inst_name && ctx.tokens[j].type != token_type::comma)
				throw AMASM_PARSER_WRONG_INST_ARGS_DEFINITION;

			if (match_rule("direct", ctx.tokens, j + 1)) {
				auto [var_offset, delta] = calc_offset(get_data_type(on_push.name), ctx.tokens, j + 3);
				arg = {
					var_offset ? argument_type::indirect_with_displacement : argument_type::direct,
					ctx.tokens[j + 2].literal,
					var_offset
				};
				dj = delta + 3;
			} else if (ctx.tokens[j + 1].type == token_type::number) {
				arg = {
					argument_type::immediate,
					"",
					std::stoll(ctx.tokens[j + 1].literal)
				};
				dj = 2;
			} else if (match_rule("indirect", ctx.tokens, j + 1)) {
				auto [var_offset, delta] = calc_offset(get_data_type(on_push.name), ctx.tokens, j + 4);
				arg = {
					argument_type::indirect_with_displacement,
					ctx.tokens[j + 3].literal,
					var_offset
				};

				if (ctx.tokens[j + delta + 4].type == token_type::plus) {
					arg.value += std::stoll(ctx.tokens[j + delta + 5].literal);
					delta += 2;
				} else if (ctx.tokens[j + delta + 4].type == token_type::minus) {
					arg.value -= std::stoll(ctx.tokens[j + delta + 5].literal);
					delta += 2;
				}
				dj = delta + 5;
			} else
				throw AMASM_PARSER_WRONG_INST_DEFINITION;

			on_push.args.push_back(arg);
		}

		func.scope.lines.emplace_back(std::move(on_push));
		ctx.di = j - ctx.i + 1 + (inst_info->max_args_count == 0);
	}

	void finish_parse_rbrace(parse_context_t& ctx) {
		if (ctx.queue.back().first == "struct_definition") {
			auto ntype = std::get<data_type_ptr>(ctx.queue.back().second);
			ctx.queue.pop();
			data_types.emplace(ntype->name, ntype);
		}
		ctx.di = 1;
	}

	const std::unordered_map<token_type, std::function<void(parse_context_t&)>> parse_rules = {
		{ token_type::kw_struct, start_parse_struct },
		{ token_type::kw_func, start_parse_function },
		{ token_type::kw_var, parse_variable },
		{ token_type::inst_name, parse_instruction },
		{ token_type::rbrace, finish_parse_rbrace }
	};

	byte_array_t translate_to_bytcode(translate_context_t& ctx) {
		const auto& func_info = std::get<func_info_t>(ctx.info);
		std::string label_name;

		label_name = func_info.name + '(';
		for (size_t i = 0; i != func_info.args.size() - 1; ++i)
			label_name += func_info.args[i]->name + ", ";
		label_name += func_info.args.back()->name + ')';



		return {};
	}
}

namespace alone::amasm {
	byte_array_t Parser::parse(const token_array_t& tokens) {
		byte_array_t result;
		parse_queue_t queue;
		std::unordered_map<std::string, size_t> labels;

		for (size_t i = 0, di = 0, address = 0; i != tokens.size(); i += di, di = 0) {
			if (auto it = parse_rules.find(tokens[i].type); it != parse_rules.end()) {
				parse_context_t ctx = { tokens, labels, queue, i, di };
				it->second(ctx);
			} else
				throw AMASM_PARSER_TOKEN_DOESNT_EXIST;

			if (!queue.empty()) {
				if (queue.front().first == "function_definition") {
				translate_context_t ctx = { labels, queue.front().second, address };
				auto temp = translate_to_bytcode(ctx);
				result.append_range(std::move(temp));
				queue.pop();
			}
			}
		}

		return result;
	}
}
