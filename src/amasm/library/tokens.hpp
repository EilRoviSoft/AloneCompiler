#pragma once

//std
#include <cstdint>
#include <string>
#include <vector>

namespace alone::amasm {
	class AmasmContext;

	enum class token_type : uint8_t {
        none       = 0x0,
        lparen     = '(', rparen    = ')', lbracket = '[', rbracket  = ']', lbrace = '{', rbrace = '}',
        dot        = '.', comma     = ',', colon    = ':', semicolon = ';',
        at         = '@', dollar    = '$', percent  = '%', quote     = '\"',
        plus       = '+', minus     = '-', star     = '*', slash     = '/',
        identifier = 0x80, number   = 129,
        data_type  = 130, inst_name = 131,
        kw_this    = 132, kw_var    = 133, kw_section = 134, kw_label = 135, kw_func = 136, kw_struct = 137,
        max        = 0xFF
    };

	struct token_t {
		token_type type;
        std::string literal;

		token_t();
		token_t(const token_type& type, std::string literal);
	};

	token_t make_token(const AmasmContext& ctx, const char& ch);
	token_t make_token(const AmasmContext& ctx, std::string str);

    using token_array_t = std::vector<token_t>;
}
