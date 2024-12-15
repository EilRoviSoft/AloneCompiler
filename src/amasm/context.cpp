#include "context.hpp"

namespace alone::amasm {
    void AmasmContext::init() {
        _init_defined_tokens();
    }

    void AmasmContext::_init_defined_tokens() {
        defined_tokens = {
            { "this", token_type::kw_this },
            { "var", token_type::kw_var },
            { "section", token_type::kw_section },
            { "label", token_type::kw_label },
            { "func", token_type::kw_func },
            { "struct", token_type::kw_struct },
        };
    }
}
