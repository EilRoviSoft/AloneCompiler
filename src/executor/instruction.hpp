#pragma once

//library
#include "library/types.hpp"

//executor
#include "executor/signatures.hpp"

namespace amasm::executor {
    class Instruction {
        friend class InstructionBuilder;
        friend class VirtualMachine;
    public:
        lib::inst_code id() const { return m_id; }
        ptrdiff_t memory_shift() const { return m_memory_shift; }
        const InstSignature& predicate() const { return m_predicate; }

        void operator()(const Context& ctx, const lib::args_data& data) const {
            return m_predicate(ctx, data);
        }

    protected:
        lib::inst_code m_id = 0;
        InstSignature m_predicate;
        ptrdiff_t m_memory_shift = 0;
    };
}
