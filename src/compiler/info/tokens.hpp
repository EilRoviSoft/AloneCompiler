#pragma once

//std
#include <cstdint>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

namespace amasm::compiler {
    class Context;

    enum class TokenType : uint8_t {
        None       = 0,
        LParen     = '(', RParen   = ')', LBracket  = '[', RBracket  = ']', LBrace = '{', RBrace = '}',
        Dot        = '.', Comma    = ',', Colon     = ':', Semicolon = ';',
        At         = '@', Dollar   = '$', Percent   = '%', Quote     = '\"',
        Plus       = '+', Minus    = '-', Star      = '*', Slash     = '/',
        Identifier = 128, Number   = 129, InstName  = 130, Typename  = 131,
        KwThis     = 192, KwVar    = 193, KwSection = 194, KwLabel   = 195, KwFunc = 196, KwStruct = 197,
        KwConst    = 198, KwString = 199,
        Max        = 255
    };

    struct token {
        TokenType type;
        std::string literal;

        token();
        token(const TokenType& type, std::string literal);
    };

    using TokenDict = std::unordered_map<std::string, TokenType>;

    token make_token(char ch);
    token make_token(std::string str);

    using token_vector = std::vector<token>;
    using token_span = std::span<token>;
}
