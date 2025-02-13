//std
#include <array>
#include <functional>

//library
#include "library/logger.hpp"

//compiler
#include "compiler/include.hpp"

//executor
#include "executor/context.hpp"
#include "executor/virtual_machine.hpp"

using namespace amasm;

namespace natives {
    void print_rax(const ExecutorContext& ctx, std::ostream& out) {
        auto rax = *ctx.get(RAX);
        out << rax << '\n';
    }
}

namespace unit_tests {
    void f0() {
        auto bytecode = Compiler::compile_from("example.amasm");
        auto vm = executor::VirtualMachine();

        vm.init();
        vm.add_native_func("@print_rax()", [&](const ExecutorContext& ctx) {
            natives::print_rax(ctx, lib::Logger::channel(lib::Logger::Output));
        });

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

int main() {
    lib::Logger::init();
    Compiler::init();

    unit_tests::test();

    return 0;
}
