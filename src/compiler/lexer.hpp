#pragma once

//compiler_info
#include "compiler/info/tokens.hpp"

namespace amasm::compiler {
    class Lexer {
    public:
        Lexer(Context& ctx);

        token_vector tokenize_code(const std::string& code) const;

    private:
        Context& _ctx;
    };
}
