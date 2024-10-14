#include "tokens.hpp"

//alone
#include "error_codes.hpp"
#include "general.hpp"

//alone::amasm
#include "consts.hpp"

namespace alone::amasm {
	token_t::token_t() :
		type(token_type::none) {
	}
	token_t::token_t(char c) :
		type((token_type) c),
		literal(1, c) {
	}
	token_t::token_t(std::string s) :
		literal(std::move(s)) {
		switch (check_type(literal)) {
		case literal_type::word: {
			auto temp = defined_tokens.find(literal);
			type = temp == defined_tokens.end() ? token_type::identifier : temp->second;
			break;
		}
		case literal_type::binary:
		case literal_type::decimal:
		case literal_type::hexadecimal:
		case literal_type::floating:
			type = token_type::number;
			break;
		default:
			throw AMASM_TOKENS_WRONG_INPUT;
		}
	}
}
