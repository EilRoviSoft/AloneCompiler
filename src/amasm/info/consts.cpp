#include "consts.hpp"

//std
#include <ranges>

//alone
#include "instructions.hpp"

//alone::amasm
#include "lexer.hpp"

namespace alone::amasm {
    std::unordered_set<char> singular_tokens;
    std::unordered_map<std::string, token_type> defined_tokens;
    std::unordered_map<std::string, data_type_ptr> data_types;
    std::unordered_map<std::string, std::vector<token_type>> rules;

    void add_data_type(const data_type_ptr& ptr) {
        if (data_types.contains(ptr->name))
            throw std::runtime_error("consts.cpp: This data_type already exists.");
        data_types.emplace(ptr->name, ptr);
        defined_tokens.emplace(ptr->name, token_type::data_type);
    }
    void init_singular_tokens() {
        for (char c : "()[]{}.,:;@$%\"+-*/")
            singular_tokens.insert(c);
    }
    void init_defined_tokens() {
        defined_tokens = {
            { "this", token_type::kw_this },
            { "var", token_type::kw_var },
            { "section", token_type::kw_section },
            { "label", token_type::kw_label },
            { "func", token_type::kw_func },
            { "struct", token_type::kw_struct },
        };
    }
    void init_data_types() {
        data_types = {
            { "void", make_data_type("void", 0) },
            { "uint8", make_data_type("uint8", 1) },
            { "uin16", make_data_type("uin16", 2) },
            { "uint32", make_data_type("uint32", 4) },
            { "uint64", make_data_type("uint64", 8) },
            { "int8", make_data_type("int8", 1) },
            { "int16", make_data_type("int16", 2) },
            { "int32", make_data_type("int32", 4) },
            { "int64", make_data_type("int64", 8) },
            { "float32", make_data_type("float32", 4) },
            { "float64", make_data_type("float64", 8) }
        };
    }
    void init_rules() {
        using enum token_type;
        rules = {
            { "struct_definition", std::vector { kw_struct, identifier, lbrace } },
            { "pole_definition", std::vector { kw_var, percent, identifier, comma, data_type } },
            { "function_definition", std::vector { kw_func, at, identifier, lparen } },
            { "direct", std::vector { percent, identifier } },
            { "indirect", std::vector { lbracket, percent, identifier } }
        };
    }

    //TODO: dispatch instructions
    void init_consts() {
        init_singular_tokens();
        init_defined_tokens();
        init_data_types();

        for (const auto& it : data_types | std::views::keys)
            defined_tokens.emplace(it, token_type::data_type);
        for (const auto& it : lib::instructions_by_name | std::views::keys)
            defined_tokens.emplace(it, token_type::inst_name);

        init_rules();
    }
}
