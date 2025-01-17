#pragma once

//std
#include <string>
#include <unordered_map>
#include <unordered_set>

//compiler
#include "compiler/info/datatypes.hpp"
#include "compiler/info/instructions.hpp"
#include "compiler/info/tokens.hpp"
#include "compiler/info/variables.hpp"

namespace amasm::compiler {
    class Context {
    public:
        Context();

        void insert_datatype(const datatype_ptr& ptr);

        const TokenType& get_token_or(const std::string& key, const TokenType& default_token) const;
        datatype_ptr get_datatype(const std::string& key) const;
        const std::vector<TokenType>& get_rule(const std::string& key) const;
        const inst_info& get_inst(const std::string& key);

        const VariablesCollection& global_variables() const;

    private:
        std::unordered_map<std::string, TokenType> _defined_tokens;
        std::unordered_map<size_t, datatype_ptr> _datatypes;
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
