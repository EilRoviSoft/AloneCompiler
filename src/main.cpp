//std
#include <iostream>

//compiler
#include "compiler/info/context.hpp"
#include "compiler/scanner.hpp"
#include "compiler/lexer.hpp"
#include "compiler/parser.hpp"
#include "compiler/translator.hpp"

namespace unit_tests {
    void f0() {
        using namespace amasm::compiler;

        auto context = Context();
        const auto scanner = Scanner(context);
        const auto lexer = Lexer(context);
        const auto parser = Parser(context);
        const auto translator = Translator(context);

        const auto text = scanner.scan_from_file("example.amasm");
        const auto tokens = lexer.tokenize_code(text);
        const auto composed = parser.parse(tokens);
        const auto bytecode = translator.translate(composed);
    }

    void test() {
        static const std::vector container = {
            std::make_tuple(0, true, std::function(f0))
        };

        for (const auto& [id, status, f] : container)
            f();
    }
}

int main() {
    unit_tests::test();

    return 0;
}
