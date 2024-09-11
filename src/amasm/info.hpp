#pragma once

//std
#include <string>
#include <vector>

namespace alone::amasm {
	enum class token_type : uint8_t {
		none = 0x0,
		lparen = '(', rparen = ')', lbracket = '[', rbracket = ']', lbrace = '{', rbrace = '}',
		dot = '.', comma = ',', colon = ':', semicolon = ';',
		at = '@', dollar = '$', percent = '%', quote = '\"',
		plus = '+', minus = '-', star = '*', slash = '/',
		identifier = 0x80, decimal = 0x81,
		max = 0xFF
	};

	struct token_t {
		token_type type;
		std::string literal;

		token_t() :
			type(token_type::none) {
		}
		token_t(token_type t, char c) :
			type(t),
			literal(1, c) {
		}
		token_t(token_type t, std::string s) :
			type(t),
			literal(std::move(s)) {
		}
	};

	using token_array_t = std::vector<token_t>;
}
