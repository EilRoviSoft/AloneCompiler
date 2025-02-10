#pragma once

//std
#include <string>
#include <unordered_map>

//compiler_info
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_container.hpp"
#include "compiler/info/scope_proxy.hpp"

namespace amasm::compiler {
    class Context {
    public:
        Context();

        ScopeProxy get_proxy();
        const InstInfo& get_inst(const std::string& key) const;

        bool has_inst(const std::string& key) const;

        ScopeContainer release_container();

    private:
        ScopeContainer _container;
        ScopeProxy _proxy;
        std::unordered_map<size_t, InstInfo> _instructions;

        void _init_instructions();
        void _init_datatypes();
        void _init_global_variables();
        void _reset();
    };
}

namespace amasm {
    using CompilerContext = compiler::Context;
}
