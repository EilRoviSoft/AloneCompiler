#include "lexer.hpp"

//alone
#include "general.hpp"

namespace alone::amasm {
    token_array_t Lexer::tokenize_code(const std::string& code) {
        token_array_t result;
        std::string temp;

        for (auto c : code) {
            if (lib::is_alpha_numeric(c)) {
                temp += c;
            } else if (!temp.empty()) {
                result.emplace_back(temp);
                temp.clear();
            }

            if (singular_tokens.contains(c))
                result.emplace_back(c);
        }

        return result;
    }
}
