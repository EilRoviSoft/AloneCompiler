#include "tokens.hpp"

//std
#include <stdexcept>

//shared
#include "shared/literals.hpp"

//amasm
#include "amasm/info/context.hpp"

namespace alone::amasm {
    token_t::token_t() :
        type(token_type::none) {
    }
    token_t::token_t(const token_type& type, std::string literal) :
        type(type),
        literal(std::move(literal)) {
    }

    token_t make_token(const AmasmContext& ctx, const char& ch) {
        return { (token_type) ch, std::string(1, ch) };
    }

    token_t make_token(const AmasmContext& ctx, std::string str) {
        token_type result_type;

        switch (shared::check_literal_type(str)) {
        case shared::literal_type::word:
            result_type = ctx.get_token_or(str, token_type::identifier);
            break;

        case shared::literal_type::binary:
        case shared::literal_type::decimal:
        case shared::literal_type::hexadecimal:
        case shared::literal_type::floating:
            result_type = token_type::number;
            break;

        default:
            throw std::runtime_error("wrong make_token arguments");
        }

        return { result_type, std::move(str) };
    }
}
