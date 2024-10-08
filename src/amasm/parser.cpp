#include "parser.hpp"

//std
#include <memory>
#include <stack>
#include <stdexcept>
#include <variant>

//alone
#include "error_codes.hpp"

#define WRONG_MATCH -1

namespace alone::amasm {
	struct scope_info_t {
		//std::vector<instruction_t> lines;
		std::vector<variable_t> vars;
	};

	struct func_info_t {
		std::string name, full_name;
		data_type_ptr return_type;
		std::vector<data_type_ptr> args;
		scope_info_t scope;
	};

	using parse_variant_t = std::variant<data_type_ptr, func_info_t, scope_info_t>;

	const std::unordered_set<std::string> surrounded_by_braces = { "struct_definition", "function_definition" };

	byte_array_t Parser::parse(const token_array_t& tokens) {
		ptrdiff_t l;
		std::stack<std::pair<std::string, parse_variant_t>> stack;
		for (size_t i = 0, delta = 0; i != tokens.size(); i += delta, delta = 0) {
			if (tokens[i].type == token_type::kw_struct) {
				if (match_rules("struct_definition", tokens, i))
					throw AMASM_PARSER_WRONG_STRUCT_DEFINITION;

				data_type_ptr on_push = make_data_type(tokens[i + 1].literal, 0);
				stack.emplace("struct_definition", std::move(on_push));
				delta = 3;
				continue;
			}

			if (tokens[i].type == token_type::kw_var) {
				if (stack.top().first == "struct_definition") {
					if (match_rules("pole_definition", tokens, i))
						throw AMASM_PARSER_WRONG_POLE_DEFINITION;

					auto& ntype = std::get<data_type_ptr>(stack.top().second);
					bool has_own_offset = tokens[i + 5].type == token_type::comma;

					ntype->add_pole(tokens[i + 2].literal, data_types.at(tokens[i + 4].literal), has_own_offset ? std::stoull(tokens[i + 10].literal) : 0);
					delta = has_own_offset ? 11 : 5;
				} else {}
				continue;
			}

			if (tokens[i].type == token_type::kw_func) {
				if (!match_rules("function_definition", tokens, i))
					throw AMASM_PARSER_WRONG_FUNC_DEFINITION;
			}

			if (tokens[i].type == token_type::rbrace) {
				if (stack.top().first == "struct_definition") {
					auto& ntype = std::get<data_type_ptr>(stack.top().second);

					data_types.emplace(ntype->name, ntype);
					stack.pop();
					delta = 1;
				}
			}
		}

		return {};
	}
	bool Parser::match_rules(const std::string& rule_name, const token_array_t& tokens, size_t start_idx) {
		const auto& pattern = rules.at(rule_name);
		bool result = true;

		for (size_t i = 0; result && i != pattern.size(); ++i)
			result = pattern[i] == tokens[start_idx + i].type;

		return result;
	}
}
