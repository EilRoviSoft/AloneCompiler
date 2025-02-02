#pragma once

//std
#include <unordered_set>

//compiler
#include "compiler/context.hpp"

//compiler_info
#include "compiler/info/tokens.hpp"

namespace amasm::compiler {
    class Lexer {
    public:
        explicit Lexer(Context& ctx);

        token_vector tokenize_code(const std::string& code) const;

    private:
        Context& _ctx;

        std::unordered_set<char> _singular_tokens;
    };
}
