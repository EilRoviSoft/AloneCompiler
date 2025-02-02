#include "context.hpp"

//library
#include "library/general_functions.hpp"

namespace amasm::compiler {
    // constructors

    Context::Context() :
        _scope(),
        _global_proxy(_scope) {
    }

    // getters

    ScopeProxy& Context::global_proxy() {
        return _global_proxy;
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
        _global_proxy.add_element(DatatypeBuilder().name("void").size(0).build());
        _global_proxy.add_element(DatatypeBuilder().name("uint8").size(1).build());
        _global_proxy.add_element(DatatypeBuilder().name("uint16").size(2).build());
        _global_proxy.add_element(DatatypeBuilder().name("uint32").size(4).build());
        _global_proxy.add_element(DatatypeBuilder().name("uint64").size(8).build());
        _global_proxy.add_element(DatatypeBuilder().name("int8").size(1).build());
        _global_proxy.add_element(DatatypeBuilder().name("int16").size(2).build());
        _global_proxy.add_element(DatatypeBuilder().name("int32").size(4).build());
        _global_proxy.add_element(DatatypeBuilder().name("int64").size(8).build());
    }
    void Context::_init_rules() {
    }
    void Context::_init_surrounded_by_braces() {
    }
    void Context::_init_instructions() {
    }
    void Context::_init_global_variables() {
    }
}
