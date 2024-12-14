#include "virtual_machine.hpp"

//alone
#include "instructions.hpp"

namespace alone::vm {
    //VirtualMachine

    VirtualMachine::VirtualMachine() :
        ctx(),
        _p0(),
        _p1() {
    }

    void VirtualMachine::exec(const lib::Bytecode& program) {
        ctx.program_size = program.size();
        for (size_t i = lib::machine_word_size; i < ctx.program_size; i++)
            _p0[i + lib::registers_size] = program[i];

        ctx.init_spans(*this);
        ctx.init_registers();
        ctx.ipx() = program.get<lib::machine_word_t>(0);

        while (ctx.flags()[(size_t) lib::flags_set::rf] && ctx.ipx() < lib::registers_size + ctx.program_size) {
            const auto& inst = *ctx.get<lib::inst_code_t>(ctx.ipx());
            auto [opcode, args_metadata] = lib::decompose_instruction(inst);
            const size_t offset = lib::instructions_by_code.at(opcode)->pred(ctx, args_metadata);
            ctx.ipx() += offset;
        }
    }
}
