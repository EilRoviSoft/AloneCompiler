#pragma once

//std
#include <functional>

//library
#include "library/types.hpp"

namespace amasm::executor {
    class Context;

    class Instruction {
        friend class VirtualMachine;
    public:
        using Signature = std::function<void(const Context&, lib::args_data)>;

        explicit Instruction(lib::inst_code id, Signature pred, ptrdiff_t memory_shift);

        void operator()(const Context& ctx, const lib::args_data& data) const;

    protected:
        lib::inst_code _id;
        Signature _pred;
        ptrdiff_t _memory_shift;
    };
    using InstructionPair = std::pair<size_t, Instruction>;

    InstructionPair make_instruction(lib::inst_code id, Instruction::Signature pred, ptrdiff_t memory_shift = 0);
}
