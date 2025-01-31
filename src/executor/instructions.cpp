#include "instructions.hpp"

//executor
#include "executor/context.hpp"

namespace amasm::executor {
    Instruction::Instruction(lib::inst_code id, Signature pred, ptrdiff_t memory_shift) :
        _id(id),
        _pred(std::move(pred)),
        _memory_shift(memory_shift) {
    }

    void Instruction::operator()(const Context& ctx, const lib::args_data& data) const {
        return _pred(ctx, data);
    }

    InstructionPair make_instruction(lib::inst_code id, Instruction::Signature pred, ptrdiff_t memory_shift) {
        return std::make_pair(id, Instruction(id, std::move(pred), memory_shift));
    }
}
