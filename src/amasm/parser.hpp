#pragma once

//shared
#include "shared/bytecode.hpp"

//amasm
#include "amasm/tokens.hpp"

namespace alone::amasm {
    class AmasmContext;

    class Parser {
    public:
        explicit Parser(const AmasmContext& amasm_ctx);

        shared::Bytecode parse(const token_array_t& tokens);

    private:
        const AmasmContext& _amasm_context;
    };
}
