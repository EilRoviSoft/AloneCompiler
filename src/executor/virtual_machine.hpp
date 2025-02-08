#pragma once

//std
#include <unordered_map>
#include <vector>

//library
#include "library/bytecode.hpp"
#include "library/types.hpp"

//executor
#include "executor/instruction.hpp"

namespace amasm::executor {
    class Context;

    class VirtualMachine {
        friend class Context;

    public:
        void init();
        void init(lib::machine_word size);
        void init(lib::machine_word min_size, lib::machine_word max_size);

        void exec(lib::Bytecode program);

    private:
        using StepperSignature = std::function<void(VirtualMachine*, const Context&)>;

        std::vector<std::byte> _mframe;
        std::unordered_map<lib::address, std::vector<std::byte>> _dframe;
        std::unordered_map<size_t, Instruction> _instructions;
        lib::machine_word _min_size, _max_size;

        void _init(lib::machine_word min_size, lib::machine_word max_size);
        void _init_memory(lib::machine_word min_size, lib::machine_word max_size);
        void _init_instructions();

        void _do_step(const Context& ctx);
        void _do_step_with_check(const Context& ctx);

        size_t _alloc_more_memory();
    };
}
