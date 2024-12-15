#include "lexer.hpp"

//std
#include <string>

//shared
#include "shared/general_functions.hpp"

//amasm
#include "amasm/context.hpp"

namespace alone::amasm {
    void Lexer::init() {
        for (char c : "()[]{}.,:;@$%\"+-*/")
            _singular_tokens.insert(c);
    }

    token_array_t Lexer::tokenize_code(const AmasmContext& ctx, const std::string& code) const {
        token_array_t result;
        std::string temp;

        for (const auto& c : code) {
            if (shared::is_alpha_numeric(c)) {
                temp += c;
            } else if (!temp.empty()) {
                result.emplace_back(make_token(ctx, temp));
                temp.clear();
            }

            if (_singular_tokens.contains(c))
                result.emplace_back(make_token(ctx, c));
        }

        return result;
    }
}
