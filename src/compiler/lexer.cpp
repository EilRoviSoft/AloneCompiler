#include "compiler/lexer.hpp"

//std
#include <string>

//frozen
#include "frozen/unordered_set.h"

//lib
#include "library/general_functions.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler::lexer {
    static constexpr frozen::unordered_set<char, 18> basic_characters = {
        '(', ')', '[', ']', '{', '}',
        '.', ',', ':', ';',
        '@', '$', '%', '\"',
        '+', '-', '*', '/'
    };
}

namespace amasm::compiler {
    token_vector Lexer::tokenize(const std::string& code) {
        token_vector result;
        std::string temp;

        for (char c : code) {
            if (lib::is_alpha_numeric(c)) {
                temp += c;
            } else if (!temp.empty()) {
                result.emplace_back(make_token(temp));
                temp.clear();
            }

            if (lexer::basic_characters.contains(c))
                result.emplace_back(make_token(c));
        }

        return result;
    }
}
