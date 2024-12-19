#include "parser.hpp"

namespace alone::amasm {
    Parser::Parser(const AmasmContext& amasm_ctx) :
        _amasm_context(amasm_ctx) {
    }

    shared::Bytecode Parser::parse(const token_array_t& tokens) {
        return {};
    }
}
