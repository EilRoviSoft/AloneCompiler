#pragma once

//std
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//compiler_info
#include "compiler/info/instructions.hpp"
#include "compiler/info/scope.hpp"
#include "compiler/info/scope_proxy.hpp"
#include "compiler/info/tokens.hpp"

namespace amasm::compiler {
    class Context {
    public:
        Context();

        ScopeProxy& global_proxy();
        const std::unordered_map<std::string, TokenType>& get_defined_tokens() const;
        const std::vector<TokenType>& get_rule(const std::string& key) const;
        const InstInfo& get_inst(const std::string& key);

    private:
        Scope _scope;
        ScopeProxy _global_proxy;
        std::unordered_map<std::string, TokenType> _defined_tokens;
        std::unordered_map<std::string, std::vector<TokenType>> _rules;
        std::unordered_set<std::string> _surrounded_by_braces;
        std::unordered_map<size_t, InstInfo> _instructions;

        void _init();
        void _init_defined_tokens();
        void _init_datatypes();
        void _init_rules();
        void _init_surrounded_by_braces();
        void _init_instructions();
        void _init_global_variables();
    };
}

namespace amasm {
    using CompilerContext = compiler::Context;
}
