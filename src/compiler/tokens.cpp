#include "tokens.hpp"

//std
#include <stdexcept>

//shared
#include "shared/literals.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler {
    token::token() :
        type(TokenType::None) {
    }
    token::token(const TokenType& type, std::string literal) :
        type(type),
        literal(std::move(literal)) {
    }

    token make_token(const Context& ctx, char ch) {
        return { (TokenType) ch, std::string(1, ch) };
    }

    token make_token(const Context& ctx, std::string str) {
        TokenType type;

        switch (shared::check_literal_type(str)) {
        case shared::LiteralType::Word:
            type = ctx.get_token_or(str, TokenType::Identifier);
            break;

        case shared::LiteralType::Binary:
        case shared::LiteralType::Decimal:
        case shared::LiteralType::Hexadecimal:
        case shared::LiteralType::Float:
            type = TokenType::Number;
            break;

        default:
            throw std::runtime_error("wrong make_token arguments");
        }

        return { type, std::move(str) };
    }
}
