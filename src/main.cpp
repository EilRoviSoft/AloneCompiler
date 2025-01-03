//amasm
#include "amasm/info/context.hpp"
#include "amasm/scanner.hpp"
#include "amasm/lexer.hpp"
#include "amasm/parser.hpp"

using namespace alone;

namespace alone::tests {
    void f0() {
        auto context = amasm::Context();
        const auto scanner = amasm::Scanner(context);
        const auto lexer = amasm::Lexer(context);
        const auto parser = amasm::Parser(context);

        const auto text = scanner.scan("example.amasm");
        const auto tokens = lexer.tokenize_code(text);
        const auto bytecode = parser.parse(tokens);
    }
}

int main() {


    return 0;
}
