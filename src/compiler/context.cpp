#include "context.hpp"

namespace amasm::compiler {
    Context::Context() {}

    ScopeProxy Context::create_proxy() {
        return ScopeProxy(_scope);
    }

    const std::unordered_map<std::string, TokenType>& Context::get_defined_tokens() const {
        return _defined_tokens;
    }
    const std::vector<TokenType>& Context::get_rule() const {
    }
    const InstInfo& Context::get_inst(const std::string& key) {
    }
}
