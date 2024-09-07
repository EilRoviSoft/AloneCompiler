#pragma once

//std
#include <map>
#include <set>
#include <string>

namespace alone::amasm::info {
	enum token_type {
		lparen, rparen, lbracket, rbracket,
		dot, comma, colon, at, percent, quote,
		plus, minus, star, slash,
		sector, label, function, container,
		instruction, identifier, data_type, string,
		integer, floating, hexadecimal, binary,
		none
	};

	struct token_t {
		std::string v;
		token_type t;

		token_t() : t(none) {}
		token_t(char ch, token_type type) : v(std::string(1, ch)), t(type) {}
		token_t(std::string str, token_type type) : v(std::move(str)), t(type) {}
	};

	using token_array_t = std::vector<token_t>;
	using token_dispatcher_t = std::function<token_t(const std::string& s, size_t& i)>;

	inline std::set<std::string> data_types = {
		"uint8", "uint16", "uint32", "uint64",
		"int8", "int16", "int32", "int64",
		"float32", "float64"
	};
	inline std::set<std::string> instructions;
	inline std::map<std::string, token_type> identifiers = {
		{ "sector", sector },
		{ "label", label },
		{ "func", function },
		{ "struct", container }
	};
}
