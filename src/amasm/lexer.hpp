#pragma once

//std
#include <unordered_set>

//alone::amasm::info
#include "amasm/info/context.hpp"
#include "amasm/info/tokens.hpp"

namespace alone::amasm {
    class Lexer {
    public:
        explicit Lexer(Context& ctx);

        token_array_t tokenize_code(const std::string& code) const;

    private:
        Context& _ctx;

        std::unordered_set<char> _singular_tokens;
    };
}
