//std
#include <fstream>
#include <functional>
#include <vector>

//compiler
#include "compiler/context.hpp"
#include "compiler/lexer.hpp"
#include "compiler/parser.hpp"
#include "compiler/scanner.hpp"
#include "compiler/translator.hpp"

//executor
#include "executor/virtual_machine.hpp"

//library
#include "library/logger.hpp"

using namespace amasm;

namespace unit_tests {
    void f0() {
        auto compiler_ctx = CompilerContext();
        auto scanner = compiler::Scanner();
        auto lexer = compiler::Lexer();
        auto parser = compiler::Parser(compiler_ctx);
        auto translator = compiler::Translator();

        auto text = scanner.scan_from_file("example.amasm");
        auto tokens = lexer.tokenize_code(text);
        auto container = parser.parse(tokens);
        auto bytecode = translator.translate(container);

        auto vm = executor::VirtualMachine();

        vm.init();
        vm.exec(bytecode);
    }

    void test() {
        const std::array container = {
            std::make_tuple(0, 1, std::function(f0))
        };

        auto is_active = [](const std::tuple<int, int, std::function<void()>>& elem) { return std::get<1>(elem); };
        for (const auto& [id, status, func] : container | std::views::filter(is_active))
            func();
    }
}

std::ostream* lib::Logger::_out = nullptr;

int main() {
    std::fstream log_file("log.txt", std::ios::out | std::ios::trunc);
    lib::Logger::init(log_file);

    unit_tests::test();

    log_file.close();

    return 0;
}
