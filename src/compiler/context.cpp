#include "context.hpp"

//std
#include <array>

//library
#include "library/general_functions.hpp"

//compiler_info
#include "compiler/info/datatype_builder.hpp"
#include "compiler/info/function_builder.hpp"
#include "compiler/info/instruction_factory.hpp"
#include "compiler/info/variable_builder.hpp"

namespace amasm::compiler {
    // constructors

    Context::Context() :
        _proxy(_scope) {
        _init();
    }

    // getters

    ScopeProxy& Context::global_proxy() {
        return _proxy;
    }
    const std::unordered_map<std::string, TokenType>& Context::get_defined_tokens() const {
        return _defined_tokens;
    }
    const std::vector<TokenType>& Context::get_rule(const std::string& key) const {
        return _rules.at(key);
    }
    const InstInfo& Context::get_inst(const std::string& key) {
        return _instructions.at(lib::hash_string(key));
    }

    // initializers

    void Context::_init() {
        _init_defined_tokens();
        _init_datatypes();
        _init_rules();
        _init_surrounded_by_braces();
        _init_instructions();
        _init_global_variables();
    }

    void Context::_init_defined_tokens() {
        _defined_tokens = {
            { "this", TokenType::KwThis },
            { "var", TokenType::KwVar },
            { "section", TokenType::KwSection },
            { "label", TokenType::KwLabel },
            { "func", TokenType::KwFunc },
            { "struct", TokenType::KwStruct },
        };
    }
    void Context::_init_datatypes() {
        _proxy.add(DatatypeBuilder().name("void").size(0).build());
        _proxy.add(DatatypeBuilder().name("uint8").size(1).build());
        _proxy.add(DatatypeBuilder().name("uint16").size(2).build());
        _proxy.add(DatatypeBuilder().name("uint32").size(4).build());
        _proxy.add(DatatypeBuilder().name("uint64").size(8).build());
        _proxy.add(DatatypeBuilder().name("int8").size(1).build());
        _proxy.add(DatatypeBuilder().name("int16").size(2).build());
        _proxy.add(DatatypeBuilder().name("int32").size(4).build());
        _proxy.add(DatatypeBuilder().name("int64").size(8).build());

        for (const auto& v : _proxy.get_all_datatypes())
            _defined_tokens.emplace(v->name(), TokenType::Datatype);
    }
    void Context::_init_rules() {
        using enum TokenType;
        _rules = {
            { "struct_define", { KwStruct, Identifier, LBrace } },
            { "pole_define", { KwVar, Identifier, Comma, Datatype } },
            { "func_define", { KwFunc, At, Identifier, LParen } },
            { "direct_argument", { Identifier } },
            { "indirect_argument", { LBracket, Identifier } }
        };
    }
    void Context::_init_surrounded_by_braces() {
        _surrounded_by_braces = {
            "struct_define", "func_define"
        };
    }
    void Context::_init_instructions() {
        auto inst_collection = InstInfoFactory().generate_info().make();

        for (const auto& v : inst_collection)
            _defined_tokens.emplace(v.name(), TokenType::InstName);

        for (auto&& it : inst_collection) {
            size_t hashed_key = it.hash();
            _instructions.emplace(hashed_key, std::move(it));
        }
    }
    void Context::_init_global_variables() {
        const std::array vars = {
            std::make_pair<std::string, std::vector<std::string>>("uint8",
                { "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh" }),
            std::make_pair<std::string, std::vector<std::string>>("uint16",
                { "ax", "bx", "cx", "dx", "ipx", "bpx", "spx", "flags", "gpx" }),
            std::make_pair<std::string, std::vector<std::string>>("uint32",
                { "eax", "ebx", "ecx", "edx", "eip", "ebp", "esp", "eflags", "egp" }),
            std::make_pair<std::string, std::vector<std::string>>("uint64",
                { "rax", "rbx", "rcx", "rdx", "rip", "rbp", "rsp", "rflags", "rgp" }),
        };
        for (const auto& [type_name, collection] : vars)
            for (const auto& var_name : collection) {
                const auto& type = _proxy.get_datatype(type_name);
                auto var = VariableBuilder()
                          .name(var_name)
                          .address(lib::str_to_register(var_name))
                          .datatype(type)
                          .build();
                _proxy.add(std::move(var));
            }
    }
}
