//amasm
#include <iostream>

#include "amasm/info/context.hpp"
#include "amasm/scanner.hpp"
#include "amasm/lexer.hpp"
#include "amasm/parser.hpp"
#include "amasm/translator.hpp"

using namespace alone;

namespace alone::unit_tests {
    void f0() {
        auto context = amasm::Context();
        const auto scanner = amasm::Scanner(context);
        const auto lexer = amasm::Lexer(context);
        const auto parser = amasm::Parser(context);
        const auto translator = amasm::Translator(context);

        const auto text = scanner.scan_from_file("example.amasm");
        const auto tokens = lexer.tokenize_code(text);
        const auto composed = parser.parse(tokens);
        const auto bytecode = translator.translate(composed);
    }

    void test() {
        static const std::vector container = {
            std::make_tuple(0, true, std::function(f0))
        };

        for (const auto& [id, status, f] : container) {
            std::cout << "test " << id;
            f();
            std::cout << " is done\n";
        }
    }
}

int main() {
    unit_tests::test();

    return 0;
}
