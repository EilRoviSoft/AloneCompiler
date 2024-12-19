#pragma once

//std
#include <unordered_set>

//alone::amasm::info
#include "amasm/tokens.hpp"

namespace alone::amasm {
    class AmasmContext;

    class Lexer {
    public:
        explicit Lexer(const AmasmContext& amasm_context);

        token_array_t tokenize_code(const std::string& code) const;

    private:
        const AmasmContext& _amasm_context;

        std::unordered_set<char> _singular_tokens;
    };
}
