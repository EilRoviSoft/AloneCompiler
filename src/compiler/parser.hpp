#pragma once

//compiler_info
#include "compiler/info/scope_container.hpp"
#include "compiler/info/tokens.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler {
    class Parser {
    public:
        static ScopeContainer parse(Context& ctx, token_vector tokens);
        // puts everything in one function and rearranges data
        static ScopeContainer simplify(ScopeContainer&& container);
    };
}
