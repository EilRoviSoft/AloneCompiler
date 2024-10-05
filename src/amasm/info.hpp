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
	enum class parse_rule_type {
		none = 0,
		singular_token,
		literal,
		number,
		sequence,
		flag
	};
	enum class parse_flag_type {
		none = 0,
		optional,
		variant,
		skip_until_next
	};

	struct token_t {
		token_type type;
		std::string literal;

		token_t();
		explicit token_t(char c);
		explicit token_t(std::string s);
	};

	using token_array_t = std::vector<token_t>;

	struct parse_rule_t;
	using parse_rule_ptr = std::shared_ptr<parse_rule_t>;

	struct singular_token_rule_t {
		token_type c;
	};
	struct literal_rule_t {
		std::string c;
	};
	struct number_rule_t {
		size_t c;
	};
	struct sequence_rule_t {
		std::vector<parse_rule_ptr> c;
	};
	struct parse_flag_rule_t {
		parse_flag_type c;
	};

	struct parse_rule_t {
		std::variant<singular_token_rule_t, literal_rule_t, number_rule_t, sequence_rule_t, parse_flag_rule_t> data;
		parse_rule_type type;

		explicit parse_rule_t(token_type nc);
		explicit parse_rule_t(const std::string& nc);
		explicit parse_rule_t(size_t nc);
		explicit parse_rule_t(const std::vector<std::string>& nc);
		//nc = [optional, variant, skip_until_next]
		explicit parse_rule_t(parse_flag_type nc);

		const token_type& get_token();
		const std::string& get_literal();
		const size_t& get_number();
		const std::vector<parse_rule_ptr>& get_sequence();
		const parse_flag_type& get_flag();

		static size_t get_length(const parse_rule_ptr& ptr);
	};

	template<typename T>
	parse_rule_ptr make_rule(T arg) {
		return std::make_shared<parse_rule_t>(arg);
	}

	extern std::unordered_set<parse_flag_type> flags_with_size;
	extern std::unordered_set<char> singular_tokens;
	extern std::unordered_map<std::string, token_type> keyword_tokens;
	extern std::unordered_map<std::string, parse_rule_ptr> rules_collection;

	void init_consts();
}
