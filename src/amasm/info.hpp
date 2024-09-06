#pragma once
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
}