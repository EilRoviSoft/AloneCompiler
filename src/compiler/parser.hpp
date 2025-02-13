#pragma once

//std
#include <optional>
#include <tuple>

//compiler_info
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_container.hpp"
#include "compiler/info/tokens.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler {
    class Parser {
    public:
        ScopeContainer parse(Context& ctx, const token_vector& tokens);

    private:
        Context* _ctx = nullptr;
        size_t _current_scope_id = 0;
        ScopeProxy _scopes;
        token_vector _tokens;

        size_t _do_parse_logic(size_t i);

        size_t _parse_datatype(size_t i);
        size_t _parse_function(size_t i);

        std::tuple<size_t, InstDecl> _parse_ncall(size_t i, const InstInfo& info);
        std::tuple<size_t, InstDecl> _parse_fcall(size_t i, const InstInfo& info);
        std::tuple<size_t, InstDecl> _parse_inst(size_t i, const InstInfo& info);

        std::tuple<size_t, Pole> _parse_pole_decl(size_t i);
        // if it's global scope, address have to be specified
        std::tuple<size_t, Variable, std::optional<InstDecl>> _parse_variable(size_t i);
    };
}
