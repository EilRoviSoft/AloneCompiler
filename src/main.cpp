//std
#include <array>
#include <chrono>
#include <functional>
#include <iostream>

//library
#include "library/logger.hpp"

//compiler
#include "compiler/include.hpp"

//executor
#include "executor/context.hpp"
#include "executor/virtual_machine.hpp"

using namespace amasm;

std::istringstream input_stream("10 20");

namespace natives {
    void read(const ExecutorContext& ctx, std::istream& in) {
        lib::machine_word value;
        in >> value;
        *ctx.get_direct<lib::machine_word>(RDI) = value;
    }
    void print(const ExecutorContext& ctx, std::ostream& out) {
        auto value = *ctx.get_direct<lib::machine_word>(RDI);
        out << value << '\n';
    }
}

namespace unit_tests {
    void f0() {
        auto bytecode = Compiler::process_from("example.amasm");
        auto vm = executor::VirtualMachine();

        vm.init();
        vm.add_native_func("@read(uint64)", [&](const ExecutorContext& ctx) {
            natives::read(ctx, input_stream);
        });
        vm.add_native_func("@print()", [&](const ExecutorContext& ctx) {
            natives::print(ctx, std::cout);
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

    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    {
        auto t1 = high_resolution_clock::now();
        unit_tests::test();
        auto t2 = high_resolution_clock::now();

        duration<double, std::milli> ms_double = t2 - t1;
        std::cout << '\n' << ms_double << '\n';
    }

    return 0;
}
