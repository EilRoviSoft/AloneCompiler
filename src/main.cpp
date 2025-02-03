//std
#include <fstream>
#include <functional>
#include <vector>

//compiler
#include "compiler/context.hpp"
//#include "compiler/lexer.hpp"
//#include "compiler/parser.hpp"
//#include "compiler/scanner.hpp"
//#include "compiler/translator.hpp"

//executor
//#include "executor/context.hpp"
//#include "executor/virtual_machine.hpp"

namespace unit_tests {
    void f0() {
        using namespace amasm;

        auto compiler_ctx = CompilerContext();
        //const auto scanner = compiler::Scanner(compiler_ctx);
        //const auto lexer = compiler::Lexer(compiler_ctx);
        //const auto parser = compiler::Parser(compiler_ctx);
        //const auto translator = compiler::Translator(compiler_ctx);

        //const auto text = scanner.scan_from_file("example.amasm");
        //const auto tokens = lexer.tokenize_code(text);
        //const auto composed = parser.parse(tokens);
        //const auto bytecode = translator.translate(composed);

        //auto vm = executor::VirtualMachine();

        //vm.init();
        //vm.exec(bytecode);
    }
    void f1() {
        std::initializer_list<std::string> regs = {
            "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh",
            "ax", "bx", "cx", "dx", "ipx", "bpx", "spx", "flags", "gpx",
            "eax", "ebx", "ecx", "edx", "eip", "ebp", "esp", "eflags", "egx",
            "rax", "rbx", "rcx", "rdx", "rip", "rbp", "rsp", "rflags", "rgx"
        };
        auto to_upper = [](std::string input) {
            for (auto& it : input)
                if (!std::isupper(it))
                    it = std::toupper(it);
            return input;
        };

        std::fstream file("temp.txt", std::ios::out | std::ios::trunc);
        for (const auto& it : regs) {
            auto upper = to_upper(it);
            file << std::vformat("{{ \"{0}\", {1} }},\n", std::make_format_args(it, upper));
        }
    }

    void test() {
        static const std::vector container = {
            std::make_tuple(0, 1, std::function(f0)),
            std::make_tuple(1, 0, std::function(f1))
        };

        for (const auto& [id, status, f] : container)
            f();
    }
}

int main() {
    unit_tests::test();

    return 0;
}
