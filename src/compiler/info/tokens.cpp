#include "tokens.hpp"

//std
#include <stdexcept>

//lib
#include "library/literals.hpp"

namespace amasm::compiler {
    token::token() :
        type(TokenType::None) {
    }
    token::token(const TokenType& type, std::string literal) :
        type(type),
        literal(std::move(literal)) {
    }

    token make_token(const TokenDict& dict, char ch) {
        return { (TokenType) ch, std::string(1, ch) };
    }

    token make_token(const TokenDict& dict, std::string str) {
        TokenType type;

        switch (lib::check_literal_type(str)) {
        case lib::LiteralType::Word:
            type = [&] {
                const auto it = dict.find(str);
                return it != dict.end() ? it->second : TokenType::Identifier;
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
