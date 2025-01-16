#include "Tokens.hpp"

//std
#include <stdexcept>

//shared
#include "shared/Literals.hpp"

//compiler
#include "compiler/info/context.hpp"

namespace amasm::compiler {
    token_t::token_t() :
        type(Tokens::None) {
    }
    token_t::token_t(const Tokens& type, std::string literal) :
        type(type),
        literal(std::move(literal)) {
    }

    token_t make_token(const Context& ctx, char ch) {
        return { (Tokens) ch, std::string(1, ch) };
    }

    token_t make_token(const Context& ctx, std::string str) {
        Tokens type;

        switch (shared::check_literal_type(str)) {
        case shared::Literals::Word:
            type = ctx.get_token_or(str, Tokens::Identifier);
            break;

        case shared::Literals::Binary:
        case shared::Literals::Decimal:
        case shared::Literals::Hexadecimal:
        case shared::Literals::Float:
            type = Tokens::Number;
            break;

        default:
            throw std::runtime_error("wrong make_token arguments");
        }

        return { type, std::move(str) };
    }
}
