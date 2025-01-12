#include "context.hpp"

//shared
#include <ranges>

//shared
#include "shared/general_functions.hpp"
#include "shared/registers.hpp"

namespace alone::amasm {
    // constructors

    Context::Context() {
        _init();
    }

    // editors

    void Context::insert_datatype(const datatype_ptr& ptr) {
        _datatypes.emplace(shared::hash_string(ptr->name), ptr);
    }

    // getters

    const Tokens& Context::get_token_or(const std::string& key, const Tokens& default_token) const {
        const auto it = _defined_tokens.find(key);
        return it != _defined_tokens.end() ? it->second : default_token;
    }
    datatype_ptr Context::get_datatype(const std::string& key) const {
        const auto it = _datatypes.find(shared::hash_string(key));
        return it != _datatypes.end() ? it->second : nullptr;
    }
    const std::vector<Tokens>& Context::get_rule(const std::string& key) const {
        return _rules.at(key);
    }
    const inst_info_t& Context::get_inst(const std::string& key) {
        return _instructions.at(shared::hash_string(key));
    }

    // initializers

    void Context::_init() {
        _init_defined_tokens();
        _init_datatypes();
        _init_rules();
        _init_surrounded_by_braces_signatures();
        _init_instructions();
    }
    void Context::_init_defined_tokens() {
        _defined_tokens = {
            { "this", Tokens::KwThis },
            { "var", Tokens::KwVar },
            { "section", Tokens::KwSection },
            { "label", Tokens::KwLabel },
            { "func", Tokens::KwFunc },
            { "struct", Tokens::KwStruct },
        };
    }
    void Context::_init_datatypes() {
        static auto wrap = [](const datatype_ptr& ptr) {
            return std::make_pair(shared::hash_string(ptr->name), ptr);
        };
        _datatypes = {
            wrap(make_datatype("void", 0)),
            wrap(make_datatype("uint8", 1)),
            wrap(make_datatype("uint16", 2)),
            wrap(make_datatype("uint32", 4)),
            wrap(make_datatype("uint64", 8)),
            wrap(make_datatype("int8", 1)),
            wrap(make_datatype("int16", 2)),
            wrap(make_datatype("int32", 4)),
            wrap(make_datatype("int64", 8)),
            wrap(make_datatype("float32", 4)),
            wrap(make_datatype("float64", 8)),
        };
    }
    void Context::_init_rules() {
        using enum Tokens;
        _rules = {
            { "struct_define", { KwStruct, Identifier, LBrace } },
            { "pole_define", { KwVar, Percent, Identifier, Comma, Datatype } },
            { "func_define", { KwFunc, At, Identifier, LParen } },
            { "direct_argument", { Percent, Identifier } },
            { "indirect_argument", { LBracket, Percent, Identifier } }
        };
    }
    void Context::_init_surrounded_by_braces_signatures() {
        _surrounded_by_braces_signatures = {
            "struct_define", "func_define"
        };
    }
    void Context::_init_instructions() {
        auto inst_collection = generate_isa_info();

        for (auto& it : inst_collection) {
            const auto key = shared::hash_string(it.name);
            _instructions.emplace(key, std::move(it));
        }

        for (const auto& value : _instructions | std::views::values)
            _defined_tokens.emplace(value.name, Tokens::InstName);
    }
    void Context::_init_global_variables() {
        using enum shared::Registers;
        _global_variables = {
            make_variable("asx", "uint64", (shared::address_t))
        };
    }
}
