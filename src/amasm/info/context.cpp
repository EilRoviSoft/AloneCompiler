#include "context.hpp"

//shared
#include "general_functions.hpp"

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

    const token_type& Context::get_token_or(const std::string& key, const token_type& default_token) const {
        const auto it = _defined_tokens.find(key);
        return it != _defined_tokens.end() ? it->second : default_token;
    }
    datatype_ptr Context::get_datatype(const std::string& key) const {
        const auto it = _datatypes.find(shared::hash_string(key));
        return it != _datatypes.end() ? it->second : nullptr;
    }
    const std::vector<token_type>& Context::get_rule(const std::string& key) const {
        return _rules.at(key);
    }
    const inst_info_t& Context::get_inst(const std::string& key) {
        return _instructions.at(shared::hash_string(key));
    }

    // initializers

    void Context::_init() {
        _init_defined_tokens();
        _init_datatypes();
        _init_surrounded_by_braces_signatures();
        _init_instructions();
    }
    void Context::_init_defined_tokens() {
        _defined_tokens = {
            { "this", token_type::kw_this },
            { "var", token_type::kw_var },
            { "section", token_type::kw_section },
            { "label", token_type::kw_label },
            { "func", token_type::kw_func },
            { "struct", token_type::kw_struct },
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
        using enum token_type;
        _rules = {
            { "struct_define", { kw_struct, identifier, lbrace } },
            { "pole_define", { kw_var, percent, identifier, comma, datatype } },
            { "func_define", { kw_func, at, identifier, lparen } },
            { "direct_argument", { percent, identifier } },
            { "indirect_argument", { lbracket, percent, identifier } }
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
    }
}
