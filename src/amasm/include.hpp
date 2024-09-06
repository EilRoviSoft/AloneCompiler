#pragma once
//std
#include <array>
#include <functional>
#include <istream>
#include <string>
#include <map>
#include <vector>

//alone::amasm::info
#include "amasm/info.hpp"

namespace alone::amasm {
	constexpr bool is_alpha(char c);
	constexpr bool is_binary(char c);
	constexpr bool is_numeric(char c);
	constexpr bool is_hexadecimal(char c);
	constexpr bool is_alpha_numeric(char c);
	constexpr bool is_whitespace(char c);

	std::string gen_str(char a, char b);

	struct token_t {
		std::string v;
		info::token_type t;

		token_t(char ch, info::token_type type);
		token_t(const std::string& str, info::token_type type);
	};

	using token_array_t = std::vector<token_t>;

	class Scanner {
	public:
		static std::string scan(std::istream& input);
		static std::string scan(const std::string& input);
	};

	using token_dispatcher_t = std::function<token_t(const std::string& s, size_t& i)>;

	class Lexer {
	public:
		static token_array_t tokenize(const std::string& code);

	private:
		static const std::array<char, 14> _singular_characters;
		static std::map<char, token_dispatcher_t> _token_machine;
		static const std::string _alpha, _numeric, _alpha_numeric;
		static bool _is_init;

		static void _init();

		static std::pair<char, token_dispatcher_t> _gen_single_char_token_pair(char c, info::token_type t);
		static std::pair<char, token_dispatcher_t> _gen_identifier(char c);
		static std::pair<char, token_dispatcher_t> _gen_number(char c);
	};
}
