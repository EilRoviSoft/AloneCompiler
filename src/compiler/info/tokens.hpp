#pragma once

//std
#include <cstdint>
#include <string>
#include <vector>

namespace amasm::compiler {
    class Context;

    enum class TokenType : uint8_t {
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

    struct token {
        TokenType type;
        std::string literal;

        token();
        token(const TokenType& type, std::string literal);
    };

    token make_token(const Context& ctx, char ch);
    token make_token(const Context& ctx, std::string str);

    using token_vector = std::vector<token>;
}
