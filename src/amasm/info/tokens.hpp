#pragma once

//std
#include <cstdint>
#include <string>
#include <vector>

namespace alone::amasm {
    class Context;

    enum class Tokens : uint8_t {
        None       = 0x0,
        LParen     = '(', RParen   = ')', LBracket = '[', RBracket  = ']', LBrace = '{', RBrace = '}',
        Dot        = '.', Comma    = ',', Colon    = ':', Semicolon = ';',
        At         = '@', Dollar   = '$', Percent  = '%', Quote     = '\"',
        Plus       = '+', Minus    = '-', Star     = '*', Slash     = '/',
        Identifier = 0x80, Number  = 129,
        Datatype   = 130, InstName = 131,
        KwThis     = 132, KwVar    = 133, KwSection = 134, KwLabel = 135, KwFunc = 136, KwStruct = 137,
        Max        = 0xFF
    };

    struct token_t {
        Tokens type;
        std::string literal;

        token_t();
        token_t(const Tokens& type, std::string literal);
    };

    token_t make_token(const Context& ctx, char ch);
    token_t make_token(const Context& ctx, std::string str);

    using token_array_t = std::vector<token_t>;
}
