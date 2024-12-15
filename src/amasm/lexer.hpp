#pragma once

//std
#include <unordered_set>

//alone::amasm::info
#include "amasm/library/tokens.hpp"

namespace alone::amasm {
    class AmasmContext;

    class Lexer {
    public:
        void init();

        token_array_t tokenize_code(const AmasmContext& ctx, const std::string& code) const;

    private:
        std::unordered_set<char> _singular_tokens;
    };
}
