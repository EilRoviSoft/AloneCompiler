#pragma once

//std
#include <functional>

//shared
#include "shared/types.hpp"

namespace amasm::executor {
    class Context;

    class Instruction {
        friend class VirtualMachine;
    public:
        using Signature = std::function<void(const Context&, shared::args_data)>;

        explicit Instruction(shared::inst_code id, Signature pred, ptrdiff_t memory_shift);

        void operator()(const Context& ctx, const shared::args_data& data) const;

    protected:
        shared::inst_code _id;
        Signature _pred;
        ptrdiff_t _memory_shift;
    };
    using InstructionPair = std::pair<size_t, Instruction>;

    InstructionPair make_instruction(shared::inst_code id, Instruction::Signature pred, ptrdiff_t memory_shift = 0);
}
