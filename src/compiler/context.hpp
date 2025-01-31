#pragma once

//std
#include <string>
#include <unordered_map>
#include <unordered_set>

//shared
#include "shared/general_functions.hpp"

//compiler
#include "compiler/datatypes.hpp"
#include "compiler/instructions.hpp"
#include "compiler/tokens.hpp"
#include "compiler/variables.hpp"

namespace amasm::compiler {
    class Context {
    public:
        Context();

        void insert_datatype(const DatatypePtr& ptr);

        const TokenType& get_token_or(const std::string& key, const TokenType& default_token) const;
        const Datatype& get_datatype(const std::string& key) const;
        const std::vector<TokenType>& get_rule(const std::string& key) const;
        const inst_info& get_inst(const std::string& key);

        const VariablesCollection& global_variables() const;

    private:
        std::unordered_map<std::string, TokenType> _defined_tokens;
        std::unordered_map<size_t, DatatypePtr> _datatypes;
        std::unordered_map<std::string, std::vector<TokenType>> _rules;
        std::unordered_set<std::string> _surrounded_by_braces_signatures;
        std::unordered_map<size_t, inst_info> _instructions;
        VariablesCollection _global_variables;

        void _init();
        void _init_defined_tokens();
        void _init_datatypes();
        void _init_rules();
        void _init_surrounded_by_braces_signatures();
        void _init_instructions();
        void _init_global_variables();
    };
}

namespace amasm {
    using CompilerContext = compiler::Context;
}
