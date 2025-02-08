#pragma once

//std
#include <functional>

//library
#include "library/types.hpp"

namespace amasm::executor {
    class Context;

    class Instruction {
        friend class InstructionBuilder;
        friend class VirtualMachine;
    public:
        using Signature = std::function<void(const Context&, lib::args_data)>;

        lib::inst_code id() const { return m_id; }
        ptrdiff_t memory_shift() const { return m_memory_shift; }
        const Signature& predicate() const { return m_predicate; }

        void operator()(const Context& ctx, const lib::args_data& data) const {
            return m_predicate(ctx, data);
        }

    protected:
        lib::inst_code m_id = 0;
        Signature m_predicate;
        ptrdiff_t m_memory_shift = 0;
    };
    using InstructionPair = std::pair<size_t, Instruction>;

    InstructionPair make_instruction(lib::inst_code id, Instruction::Signature pred, ptrdiff_t memory_shift = 0);
}
