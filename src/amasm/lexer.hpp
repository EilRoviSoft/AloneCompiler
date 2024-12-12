#pragma once

//alone::amasm::info
#include "amasm/info/include.hpp"

namespace alone::amasm {
    class Lexer {
    public:
        static token_array_t tokenize_code(const std::string& code);
    };
}
