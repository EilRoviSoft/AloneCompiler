#pragma once

//std
#include <array>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>

//alone::amasm::info
#include "amasm/info.hpp"

namespace alone::amasm {
	class Lexer {
	public:
		static info::token_array_t tokenize(const std::string& code);

	private:
		static const std::array<char, 15> _singular_characters;
		static std::unordered_map<char, info::token_dispatcher_t> _token_machine;
		static const std::string _alpha, _numeric, _alpha_numeric;
		static bool _is_init;

		static void _init();

		static std::string _get_until(const std::string& s, size_t& i, const std::function<bool(char)>& f);
		static std::pair<char, info::token_dispatcher_t> _gen_single_char_token_pair(char c, info::token_type t);
		static std::pair<char, info::token_dispatcher_t> _gen_identifier(char c);
		static std::pair<char, info::token_dispatcher_t> _gen_number(char c);
	};
}
