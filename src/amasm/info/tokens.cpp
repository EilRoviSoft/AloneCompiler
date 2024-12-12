#include "tokens.hpp"

//alone
#include "error_codes.hpp"
#include "general.hpp"

//alone::amasm::info
#include "consts.hpp"

namespace alone::amasm {
    token_t::token_t() :
        type(token_type::none) {
    }
    token_t::token_t(char c) :
        type((token_type) c),
        literal(1, c) {
    }
    token_t::token_t(std::string s) :
        literal(std::move(s)) {
        auto literal_type = lib::check_type(literal);
        if (literal_type == lib::literal_type::word) {
            auto temp = defined_tokens.find(literal);
            type = temp == defined_tokens.end() ? token_type::identifier : temp->second;
        } else if (literal_type == lib::literal_type::binary || literal_type == lib::literal_type::decimal ||
            literal_type == lib::literal_type::hexadecimal || literal_type == lib::literal_type::floating) {
            type = token_type::number;
        } else
            throw AMASM_TOKENS_WRONG_INPUT;
    }
}
