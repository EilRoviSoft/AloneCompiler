#pragma once

//alone
#include "general.hpp"

//alone::amasm
#include "amasm/info/include.hpp"

namespace alone::amasm {
    class Parser {
    public:
        //TODO: make tree-based parsing
        static lib::Bytecode parse(const token_array_t& tokens);
    };
}
