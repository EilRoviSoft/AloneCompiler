#pragma once

//std
#include <unordered_set>

//alone::amasm::info
#include "amasm/info//tokens.hpp"

namespace alone::amasm {
    class AmasmContext;

    class Lexer {
    public:
        explicit Lexer(AmasmContext& ctx);

        token_array_t tokenize_code(const std::string& code) const;

    private:
        AmasmContext& _ctx;

        std::unordered_set<char> _singular_tokens;
    };
}
