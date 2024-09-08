#pragma once

//std
#include <unordered_map>
#include <unordered_set>
#include <string>

//alone::amasm::data_types
#include "amasm/data_types.hpp"

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
		std::string val;
		token_type type;

		token_t() :
			type(none) {
		}
		token_t(char c, token_type t) :
			val(std::string(1, c)),
			type(t) {
		}
		token_t(std::string s, token_type t) :
			val(std::move(s)), type(t) {
		}
	};

	using token_array_t = std::vector<token_t>;
	using token_dispatcher_t = std::function<token_t(const std::string& s, size_t& i)>;

	inline std::unordered_set<std::shared_ptr<data_type_t>, data_type_hasher> data_types = {
		std::make_shared<data_type_t>("uint8", 1),
		std::make_shared<data_type_t>("uint16", 2),
		std::make_shared<data_type_t>("uint32", 4),
		std::make_shared<data_type_t>("uint64", 8),
		std::make_shared<data_type_t>("int8", 1),
		std::make_shared<data_type_t>("int16", 2),
		std::make_shared<data_type_t>("int32", 4),
		std::make_shared<data_type_t>("int64", 8),
		std::make_shared<data_type_t>("float32", 4),
		std::make_shared<data_type_t>("float64", 8)
	};

	/*inline std::unordered_set<std::string> data_types = {
		"uint8", "uint16", "uint32", "uint64",
		"int8", "int16", "int32", "int64",
		"float32", "float64"
	};*/
	inline std::unordered_set<std::string> instructions;
	inline std::unordered_map<std::string, token_type> identifiers = {
		{ "sector", sector },
		{ "label", label },
		{ "func", function },
		{ "struct", container }
	};
}
