#pragma once

//std
#include <tuple>

//compiler_info
#include "compiler/info/function_builder.hpp"
#include "compiler/info/scope_container.hpp"
#include "compiler/info/tokens.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler {
    class Parser {

    public:
        explicit Parser(Context& ctx);

        ScopeContainer parse(token_vector tokens);

    private:
        Context& _ctx;
        ScopeProxy _scopes;
        token_vector _tokens;

        size_t _do_parse_logic(size_t i);

        size_t _parse_datatype(size_t i);
        size_t _parse_function(size_t i);

        std::tuple<InstDecl, size_t> _parse_fcall(size_t i, const InstInfo& info);
        std::tuple<InstDecl, size_t> _parse_inst(size_t i, const InstInfo& info);

        std::tuple<Datatype, size_t> _parse_variable(size_t i);
    };
}
