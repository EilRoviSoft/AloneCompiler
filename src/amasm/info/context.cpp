#include "context.hpp"

//shared
#include "general_functions.hpp"

namespace alone::amasm {
    void AmasmContext::init() {
        _init_defined_tokens();
        _init_datatypes();
    }

    void AmasmContext::insert_datatype(const datatype_ptr& ptr) {
        _datatypes.emplace(shared::hash_string(ptr->name), ptr);
    }

    const token_type& AmasmContext::get_token_or(const std::string& key, const token_type& default_token) const {
        const auto it = _defined_tokens.find(key);
        return it != _defined_tokens.end() ? it->second : default_token;
    }
    datatype_ptr AmasmContext::get_datatype(const std::string& key) const {
        const auto it = _datatypes.find(shared::hash_string(key));
        return it != _datatypes.end() ? it->second : nullptr;
    }

    void AmasmContext::_init_defined_tokens() {
        _defined_tokens = {
            { "this", token_type::kw_this },
            { "var", token_type::kw_var },
            { "section", token_type::kw_section },
            { "label", token_type::kw_label },
            { "func", token_type::kw_func },
            { "struct", token_type::kw_struct },
        };
    }
    void AmasmContext::_init_datatypes() {
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
}
