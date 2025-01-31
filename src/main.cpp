//compiler
#include "compiler/context.hpp"
#include "compiler/lexer.hpp"
#include "compiler/parser.hpp"
#include "compiler/scanner.hpp"
#include "compiler/translator.hpp"

//executor
#include "executor/context.hpp"
#include "executor/virtual_machine.hpp"

namespace unit_tests {
    void f0() {
        using namespace amasm;

        auto compiler_ctx = CompilerContext();
        const auto scanner = compiler::Scanner(compiler_ctx);
        const auto lexer = compiler::Lexer(compiler_ctx);
        const auto parser = compiler::Parser(compiler_ctx);
        const auto translator = compiler::Translator(compiler_ctx);

        const auto text = scanner.scan_from_file("example.amasm");
        const auto tokens = lexer.tokenize_code(text);
        const auto composed = parser.parse(tokens);
        const auto bytecode = translator.translate(composed);

        auto vm_ctx = ExecutorContext();
        auto vm = executor::VirtualMachine();

        vm.init();
        vm.exec(vm_ctx, bytecode);
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
