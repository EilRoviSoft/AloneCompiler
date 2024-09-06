#pragma once
namespace alone::amasm::info {
	enum token_type {
		lparen, rparen, lbracket, rbracket,
		dot, comma, colon, at, percent, quote,
		plus, minus, star, slash,
		keyword, instruction, identifier, string, number,
		none
	};
}