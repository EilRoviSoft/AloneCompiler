#pragma once

//std
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "info.hpp"

namespace alone::amasm {
	enum class token_type : uint8_t {
		none = 0x0,
		lparen = '(', rparen = ')', lbracket = '[', rbracket = ']', lbrace = '{', rbrace = '}',
		dot = '.', comma = ',', colon = ':', semicolon = ';',
		at = '@', dollar = '$', percent = '%', quote = '\"',
		plus = '+', minus = '-', star = '*', slash = '/',
		identifier = 0x80, number,
		max = 0xFF
	};

	struct token_t {
		token_type type;
		std::string literal;

		token_t();
		token_t(token_type t, char c);
		token_t(token_type t, std::string s);
	};

	using parse_rule_ptr = std::shared_ptr<struct parse_rule_t>;

	enum class parse_rule_flag {
		none = 0,
		optional,
		or_flag
	};

	enum class parse_rule_type {
		none = 0,
		flag,
		singular_token,
		literal,
		sequence
	};

	//TODO: multiline_code, multiple_argument dispatching, data_type and function_signature
	struct parse_rule_t {
		std::variant<parse_rule_flag, token_type, std::string, std::vector<parse_rule_ptr>> content;
		parse_rule_type type;

		//for complex rules, wrote in reversed polish notation
		parse_rule_t(parse_rule_flag f);
		//for identifiers and tokens
		parse_rule_t(token_type t);
		//for keywords and rules
		parse_rule_t(std::string s);
		//for long sequences
		parse_rule_t(const std::initializer_list<std::string>& str_v);

		token_type get_token_type() const;
		parse_rule_flag get_parse_rule_flag() const;
		const std::string& get_literal() const;
		const std::vector<parse_rule_ptr>& get_seq() const;
	};

	using token_array_t = std::vector<token_t>;

	extern std::unordered_map<std::string, parse_rule_ptr> rules;

	template<typename T>
	parse_rule_ptr make_rule(T arg) {
		return std::make_shared<parse_rule_t>(arg);
	}
}
