#pragma once

//std
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace alone::amasm {
	enum class token_type : uint8_t {
		none = 0x0,
		lparen = '(', rparen = ')', lbracket = '[', rbracket = ']', lbrace = '{', rbrace = '}',
		dot = '.', comma = ',', colon = ':', semicolon = ';',
		at = '@', dollar = '$', percent = '%', quote = '\"',
		plus = '+', minus = '-', star = '*', slash = '/',
		identifier = 0x80, number,
		data_type,
		section_keyword, label_keyword, function_keyword, struct_keyword,
		max = 0xFF
	};

	struct token_t {
		token_type type;
		std::string literal;

		token_t();
		explicit token_t(char c);
		explicit token_t(std::string s);
	};

	enum class parse_rule_flag {
		none = 0,
		optional,
		variant
	};
	enum class parse_rule_type {
		none = 0,
		flag,
		singular_token,
		literal,
		number,
		sequence
	};

	struct parse_rule_t;

	using parse_rule_ptr = std::shared_ptr<parse_rule_t>;

	//TODO: multiline_code, multiple_argument dispatching, data_type and function_signature
	struct parse_rule_t {
		std::variant<parse_rule_flag, token_type, std::string, size_t, std::vector<parse_rule_ptr>> data;
		parse_rule_type type;

		explicit parse_rule_t(parse_rule_flag ptr);
		explicit parse_rule_t(token_type token);
		explicit parse_rule_t(const std::string& str);
		explicit parse_rule_t(size_t num);
		explicit parse_rule_t(const std::vector<std::string>& str_vec);

		const parse_rule_flag& get_flag();
		const token_type& get_token();
		const std::string& get_literal();
		const size_t& get_number();
		const std::vector<parse_rule_ptr>& get_sequence();

		static size_t get_length(const parse_rule_ptr& ptr);
	};

	using token_array_t = std::vector<token_t>;

	template<typename _T>
	parse_rule_ptr make_rule(_T arg) {
		return std::make_shared<parse_rule_t>(arg);
	}

	extern std::unordered_set<char> singular_tokens;
	extern std::unordered_map<std::string, token_type> keyword_tokens;
	extern std::unordered_map<std::string, parse_rule_ptr> rules_collection;

	void init_consts();
}
