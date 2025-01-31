#include "lexer.hpp"

//std
#include <string>

//lib
#include "library/general_functions.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler {
    Lexer::Lexer(Context& ctx) :
        _ctx(ctx) {
        for (char c : "()[]{}.,:;@$%\"+-*/")
            _singular_tokens.insert(c);
    }

    token_vector Lexer::tokenize_code(const std::string& code) const {
        token_vector result;
        std::string temp;

        for (char c : code) {
            if (lib::is_alpha_numeric(c)) {
                temp += c;
            } else if (!temp.empty()) {
                result.emplace_back(make_token(_ctx, temp));
                temp.clear();
            }

            if (_singular_tokens.contains(c))
                result.emplace_back(make_token(_ctx, c));
        }

        return result;
    }
}
