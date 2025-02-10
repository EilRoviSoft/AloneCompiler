#pragma once

//compiler_info
#include "compiler/info/tokens.hpp"

namespace amasm::compiler {
    class Lexer {
    public:
        token_vector tokenize_code(const std::string& code);
    };
}
