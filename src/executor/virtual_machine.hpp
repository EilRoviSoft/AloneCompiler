#pragma once

//std
#include <unordered_map>
#include <vector>

//shared
#include "shared/bytecode.hpp"
#include "shared/types.hpp"

//executor
#include "executor/instructions.hpp"

namespace amasm::executor {
    class Context;

    class VirtualMachine {
        friend class Context;

    public:
        void init();
        void init(shared::machine_word size);
        void init(shared::machine_word min_size, shared::machine_word max_size);

        void exec(Context& ctx, shared::Bytecode program);

    private:
        using StepperSignature = std::function<void(VirtualMachine*, const Context&)>;

        std::vector<std::byte> _mframe;
        std::unordered_map<shared::address, std::vector<std::byte>> _dframe;
        std::unordered_map<size_t, Instruction> _instructions;
        shared::machine_word _min_size, _max_size;

        void _init(shared::machine_word min_size, shared::machine_word max_size);
        void _init_memory(shared::machine_word min_size, shared::machine_word max_size);
        void _init_instructions();

        void _do_step(const Context& ctx);
        void _do_step_with_check(const Context& ctx);

        size_t _alloc_more_memory();
    };
}
