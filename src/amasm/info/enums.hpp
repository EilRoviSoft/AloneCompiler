#pragma once

//std
#include <cstdint>

namespace alone::amasm {
	enum class token_type : uint8_t {
		none       = 0x0,
		lparen     = '(', rparen = ')', lbracket = '[', rbracket  = ']', lbrace = '{', rbrace = '}',
		dot        = '.', comma  = ',', colon    = ':', semicolon = ';',
		at         = '@', dollar = '$', percent  = '%', quote     = '\"',
		plus       = '+', minus  = '-', star     = '*', slash     = '/',
		identifier = 0x80, number,
		data_type, inst_name,
		kw_this, kw_var, kw_section, kw_label, kw_func, kw_struct,
		max = 0xFF
	};
}
