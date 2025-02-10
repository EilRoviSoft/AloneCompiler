#pragma once

//std
#include <string>
#include <unordered_map>

//compiler_info
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_container.hpp"
#include "compiler/info/scope_proxy.hpp"
#include "compiler/info/tokens.hpp"

namespace amasm::compiler {
    class Context {
    public:
        Context();

        ScopeProxy get_proxy();
        const std::unordered_map<std::string, TokenType>& get_defined_tokens() const;
        const InstInfo& get_inst_info(const std::string& key) const;

        ScopeContainer release_container();

    private:
        ScopeContainer _container;
        ScopeProxy _proxy;
        std::unordered_map<std::string, TokenType> _defined_tokens;
        std::unordered_map<size_t, InstInfo> _instructions;

        void _init_instructions();
        void _init_defined_tokens();
        void _init();
        void _init_datatypes();
        void _init_global_variables();
        void _reset();
    };
}

namespace amasm {
    using CompilerContext = compiler::Context;
}
