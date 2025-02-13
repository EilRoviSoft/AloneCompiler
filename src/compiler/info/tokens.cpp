#include "compiler/info/tokens.hpp"

//std
#include <stdexcept>

//frozen
#include "frozen/string.h"
#include "frozen/unordered_map.h"

//lib
#include "library/literals.hpp"

namespace amasm::compiler {
    static constexpr frozen::unordered_map<frozen::string, TokenType, 6> defined_tokens = {
        { "this", TokenType::KwThis },
        { "var", TokenType::KwVar },
        { "section", TokenType::KwSection },
        { "label", TokenType::KwLabel },
        { "func", TokenType::KwFunc },
        { "struct", TokenType::KwStruct },
    };

    token::token() :
        type(TokenType::None) {
    }
    token::token(const TokenType& type, std::string literal) :
        type(type),
        literal(std::move(literal)) {
    }

    token make_token(char ch) {
        return { (TokenType) ch, std::string(1, ch) };
    }

    token make_token(std::string str) {
        TokenType type;

        switch (lib::check_literal_type(str)) {
        case lib::LiteralType::Word:
            type = [&] {
                auto key = frozen::string(str.c_str(), str.size());
                auto it = defined_tokens.find(key);
                return it != defined_tokens.end() ? it->second : TokenType::Identifier;
            }();
            break;

        case lib::LiteralType::Binary:
        case lib::LiteralType::Decimal:
        case lib::LiteralType::Hexadecimal:
        case lib::LiteralType::Float:
            type = TokenType::Number;
            break;

        default:
            throw std::runtime_error("wrong make_token arguments");
        }

        return { type, std::move(str) };
    }
}
