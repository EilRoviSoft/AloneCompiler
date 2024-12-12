#pragma once

//std
#include <cstdint>

namespace alone::amasm {
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
}
