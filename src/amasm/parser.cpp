#include "parser.hpp"

//std
#include <memory>
#include <queue>
#include <stdexcept>
#include <variant>

//alone
#include "error_codes.hpp"
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
	using parse_queue_t = std::queue<std::tuple<std::string, bool, parse_variant_t>>;

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

	bool match_rule(const std::string& rule_name, const token_array_t& tokens, const size_t& start_idx) {
		const auto& pattern = rules.at(rule_name);
		bool result = true;

		for (size_t i = 0, diff = 0; result && i != pattern.size(); ++i)
			result = pattern[i + diff] == tokens[start_idx + i].type;

		return result;
	}
	std::tuple<ptrdiff_t, size_t> calc_offset(const data_type_ptr& type, const token_array_t& tokens, const size_t& start_idx) {
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

	void start_parse_struct(const parse_context_t& parsing_data) {
		if (!match_rule("struct_definition", parsing_data.tokens, parsing_data.i))
			throw AMASM_PARSER_WRONG_STRUCT_DEFINITION;

		data_type_ptr on_push = make_data_type(parsing_data.tokens[parsing_data.i + 1].literal, 0);
		parsing_data.di = 3;
		parsing_data.queue.emplace("struct_definition", false, std::move(on_push));
	}
	void start_parse_function(const parse_context_t& ctx) {
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
		ctx.queue.emplace("function_definition", false, std::move(on_push));
	}

	void parse_variable(const parse_context_t& parsing_data) {
		const auto& [name, is_finished, data] = parsing_data.queue.front();
		if (name == "struct_definition") {
			if (!match_rule("pole_definition", parsing_data.tokens, parsing_data.i))
				throw AMASM_PARSER_WRONG_POLE_DEFINITION;

			auto& ntype = std::get<data_type_ptr>(data);
			bool has_own_offset = parsing_data.tokens[parsing_data.i + 5].type == token_type::comma;

			ntype->add_pole(parsing_data.tokens[parsing_data.i + 2].literal, data_types.at(parsing_data.tokens[parsing_data.i + 4].literal));
			parsing_data.di = has_own_offset ? 13 : 6;
		} else {}
	}
	void parse_instruction(const parse_context_t& parsing_data) {
		auto& [name, is_finished, data] = parsing_data.queue.front();
		if (name != "function_definition" || is_finished)
			throw AMASM_PARSER_WRONG_INST_DEFINITION_PLACE;

		size_t j, dj, args_n;
		auto& func_data = std::get<func_info_t>(data);
		const auto& inst_info = lib::instructions_by_name.at(parsing_data.tokens.at(parsing_data.i).literal);
		inst_call_t on_push;

		on_push.name = parsing_data.tokens[parsing_data.i].literal;
		for (j = parsing_data.i, dj = 0, args_n = 0; args_n != inst_info->max_args_count &&
			 parsing_data.tokens[j].type != token_type::semicolon; j += dj, dj = 0, ++args_n) {
			argument_t arg;

			if (parsing_data.tokens[j].type != token_type::inst_name && parsing_data.tokens[j].type != token_type::comma)
				throw AMASM_PARSER_WRONG_INST_ARGS_DEFINITION;

			if (match_rule("direct", parsing_data.tokens, j + 1)) {
				auto [var_offset, delta] = calc_offset(get_data_type(on_push.name), parsing_data.tokens, j + 3);
				arg = {
					.type = var_offset ? lib::argument_type::indirect_with_displacement : lib::argument_type::direct,
					.name = parsing_data.tokens[j + 2].literal,
					.value = var_offset
				};
				dj = delta + 3;
			} else if (parsing_data.tokens[j + 1].type == token_type::number) {
				arg = {
					.type = lib::argument_type::immediate,
					.name = "",
					.value = std::stoll(parsing_data.tokens[j + 1].literal)
				};
				dj = 2;
			} else if (match_rule("indirect", parsing_data.tokens, j + 1)) {
				auto [var_offset, delta] = calc_offset(get_data_type(on_push.name), parsing_data.tokens, j + 4);
				arg = {
					.type = lib::argument_type::indirect_with_displacement,
					.name = parsing_data.tokens[j + 3].literal,
					.value = var_offset
				};

				if (parsing_data.tokens[j + delta + 4].type == token_type::plus) {
					arg.value += std::stoll(parsing_data.tokens[j + delta + 5].literal);
					delta += 2;
				} else if (parsing_data.tokens[j + delta + 4].type == token_type::minus) {
					arg.value -= std::stoll(parsing_data.tokens[j + delta + 5].literal);
					delta += 2;
				}
				dj = delta + 5;
			} else
				throw AMASM_PARSER_WRONG_INST_DEFINITION;

			on_push.args.push_back(arg);
		}

		func_data.scope.lines.emplace_back(std::move(on_push));
		parsing_data.di = j - parsing_data.i + 1 + (inst_info->max_args_count == 0);
	}

	void finish_parse_rbrace(const parse_context_t& parsing_data) {
		auto& [name, is_finished, data] = parsing_data.queue.front();
		if (is_finished)
			throw AMASM_PARSER_FINISHED_TOO_EARLY;

		if (name == "struct_definition") {
			auto ntype = std::get<data_type_ptr>(data);
			parsing_data.queue.pop();
			data_types.emplace(ntype->name, ntype);
		}
		is_finished = true;
		parsing_data.di = 1;
	}

	std::unordered_map<token_type, std::function<void(parse_context_t&)>> parse_rules = {
		{ token_type::kw_struct, start_parse_struct },
		{ token_type::kw_func, start_parse_function },
		{ token_type::kw_var, parse_variable },
		{ token_type::inst_name, parse_instruction },
		{ token_type::rbrace, finish_parse_rbrace }
	};

	lib::byte_array_t translate_to_bytecode(const translate_context_t& translation_data) {
		const auto& func_info = std::get<func_info_t>(translation_data.info);
		std::string label_name = func_info.name + '(';

		for (size_t i = 0; i != func_info.args.size() - 1; ++i)
			label_name += func_info.args[i]->name + ", ";
		label_name += func_info.args.back()->name + ')';

		return {};
	}
}

namespace alone::amasm {
	lib::byte_array_t Parser::parse(const token_array_t& tokens) {
		lib::byte_array_t result;
		parse_queue_t queue;
		std::unordered_map<std::string, size_t> labels;

		for (size_t i = 0, di = 0, address = 0; i != tokens.size(); i += di, di = 0) {
			if (auto it = parse_rules.find(tokens[i].type); it != parse_rules.end()) {
				parse_context_t ctx = {
					.tokens = tokens,
					.labels = labels,
					.queue = queue,
					.i = i,
					.di = di
				};
				it->second(ctx);
			} else
				throw AMASM_PARSER_TOKEN_DOESNT_EXIST;

			if (!queue.empty()) {
				const auto& [name, is_finished, data] = queue.front();
				if (is_finished && name == "function_definition") {
					translate_context_t ctx = {
						.labels = labels,
						.info = data,
						.i = address
					};
					auto temp = translate_to_bytecode(ctx);
					result.append_range(std::move(temp));
					queue.pop();
				}
			}
		}

		return result;
	}
}
