//std
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

//compiler
#include "compiler/context.hpp"
#include "compiler/lexer.hpp"
#include "compiler/parser.hpp"
#include "compiler/scanner.hpp"
//#include "compiler/translator.hpp"

//executor
//#include "executor/virtual_machine.hpp"

namespace unit_tests {
    void f0() {
        using namespace amasm;

        auto compiler_ctx = CompilerContext();
        const auto scanner = compiler::Scanner(compiler_ctx);
        const auto lexer = compiler::Lexer(compiler_ctx);
        const auto parser = compiler::Parser(compiler_ctx);
        //const auto translator = compiler::Translator(compiler_ctx);

        const auto text = scanner.scan_from_file("example.amasm");
        const auto tokens = lexer.tokenize_code(text);
        const auto scope = parser.parse(tokens);
        //const auto bytecode = translator.translate(composed);

        //auto vm = executor::VirtualMachine();

        //vm.init();
        //vm.exec(bytecode);
    }

    void test() {
        const std::array container = {
            std::make_tuple(0, 1, std::function(f0))
        };

        auto filter = [](const std::tuple<int, int, std::function<void()>>& elem) { return std::get<1>(elem); };
        for (const auto& [id, status, func] : container | std::views::filter(filter))
            func();
    }
}

int main() {
    unit_tests::test();

    return 0;
}
