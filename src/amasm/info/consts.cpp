#include "consts.hpp"

//std
#include <ranges>
#include <stdexcept>

//alone::amasm
#include "lexer.hpp"

namespace alone::amasm {
	std::unordered_set<char> singular_tokens;
	std::unordered_map<std::string, token_type> defined_tokens;
	std::unordered_map<std::string, parse_rule_ptr> rules;
	std::unordered_map<std::string, data_type_ptr> data_types;

	void add_data_type(const data_type_ptr& ptr) {
		if (data_types.contains(ptr->name))
			throw std::runtime_error("consts.cpp: This data_type already exists.");
		data_types.emplace(ptr->name, ptr);
		defined_tokens.emplace(ptr->name, token_type::data_type);
	}

	void init_consts() {
		singular_tokens = {
			'(', ')', '[', ']', '{', '}',
			'.', ',', ':', ';',
			'@', '$', '%', '\"',
			'+', '-', '*', '/'
		};
		defined_tokens = {
			{ "this",    token_type::kw_this },
			{ "var",     token_type::kw_var },
			{ "section", token_type::kw_section },
			{ "label",   token_type::kw_label },
			{ "func",    token_type::kw_func },
			{ "struct",  token_type::kw_struct },
		};
		data_types = {
			make_data_type("uint8", 1),
			make_data_type("uint16", 2),
			make_data_type("uint32", 4),
			make_data_type("uint64", 8),
			make_data_type("int8", 1),
			make_data_type("int16", 2),
			make_data_type("int32", 4),
			make_data_type("int64", 8),
			make_data_type("float32", 4),
			make_data_type("float64", 8)
		};

		for (const auto& it : data_types | std::views::filter([](const auto& it) {
			return !defined_tokens.contains(it.first);
		}) | std::views::keys) {
			defined_tokens.emplace(it, token_type::data_type);
		}

		//simple rules

		rules.emplace("?", make_rule(parse_flag_type::optional));
		rules.emplace("|", make_rule(parse_flag_type::variant));
		rules.emplace("skip_body", make_rule(parse_flag_type::skip_body));
		rules.emplace("skip_args", make_rule(parse_flag_type::skip_args));
		rules.emplace("(", make_rule(token_type::lparen));
		rules.emplace(")", make_rule(token_type::rparen));
		rules.emplace("[", make_rule(token_type::lbracket));
		rules.emplace("]", make_rule(token_type::rbracket));
		rules.emplace("{", make_rule(token_type::lbrace));
		rules.emplace("}", make_rule(token_type::rbrace));
		rules.emplace(".", make_rule(token_type::dot));
		rules.emplace(",", make_rule(token_type::comma));
		rules.emplace(":", make_rule(token_type::colon));
		rules.emplace(";", make_rule(token_type::semicolon));
		rules.emplace("@", make_rule(token_type::at));
		rules.emplace("$", make_rule(token_type::dollar));
		rules.emplace("%", make_rule(token_type::percent));
		rules.emplace("\"", make_rule(token_type::quote));
		rules.emplace("+", make_rule(token_type::plus));
		rules.emplace("-", make_rule(token_type::minus));
		rules.emplace("*", make_rule(token_type::star));
		rules.emplace("/", make_rule(token_type::slash));

		rules.emplace("identifier", make_rule(token_type::identifier));
		rules.emplace("number", make_rule(token_type::number));
		rules.emplace("data_type", make_rule(token_type::data_type));
		rules.emplace("inst_name", make_rule(token_type::inst_name));

		rules.emplace("kw_this", make_rule(token_type::kw_this));
		rules.emplace("kw_var", make_rule(token_type::kw_var));
		rules.emplace("kw_section", make_rule(token_type::kw_section));
		rules.emplace("kw_label", make_rule(token_type::kw_label));
		rules.emplace("kw_func", make_rule(token_type::kw_func));
		rules.emplace("kw_struct", make_rule(token_type::kw_struct));

		//complex rules

		//make_rule(Lexer::tokenize_rule_sequence(""))

		rules.emplace("address_with_displacement", make_rule(Lexer::tokenize_rule_sequence(
			"[%identifier |2 +- number]")));
		/*rules.emplace("var_definition", make_rule(Lexer::tokenize_rule_sequence(
			"kw_var %identifier")));*/
		/*rules.emplace("pole_offset", make_rule(Lexer::tokenize_rule_sequence(
			"[%this + number]")));*/
		rules.emplace("pole_definition", make_rule(Lexer::tokenize_rule_sequence(
			"kw_var %identifier, data_type, [%kw_this ?2 + number]")));

		rules.emplace("function_definition", make_rule(Lexer::tokenize_rule_sequence(
			"kw_func @identifier(skip_args) { skip_body }")));

		rules.emplace("struct_definition", make_rule(Lexer::tokenize_rule_sequence(
			"kw_struct identifier { skip_body }")));

		//instruction rules

		rules.emplace("inst0", make_rule(Lexer::tokenize_rule_sequence(
			"identifier")));
	}
}
