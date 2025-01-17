#include "virtual_machine.hpp"

//shared
#include "shared/general_functions.hpp"
#include "shared/types.hpp"

//executor
#include "executor/context.hpp"

namespace amasm::executor {
    void VirtualMachine::exec(const SharedContext& ctx, const shared::Bytecode& program) {
        ctx->_program_size = program.size() - shared::machine_word_size;
        for (size_t i = shared::machine_word_size; i < ctx->program_size(); i++)
            _mframe[i + shared::registers_size] = program[i];

        ctx->flags()[RF] = true;
        ctx->reg(SPX) = shared::registers_size + ctx->_program_size;
        ctx->reg(IPX) = reinterpret_cast<shared::MachineWord>(program.get(shared::machine_word_size, 0).data());
    }
}
