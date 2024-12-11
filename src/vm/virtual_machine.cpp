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

	void VirtualMachine::exec(const lib::byte_array_t& program) {
		ctx.program_size = program.size();
		for (size_t i = 0; i != ctx.program_size; ++i)
			_p0[i + lib::registers_size] = program[i];

		ctx.init_spans(*this);
		ctx.init_registers();

		while (ctx.flags()[(size_t) lib::flags_set::rf] && ctx.ipx() < lib::registers_size + ctx.program_size) {
			auto [opcode, args_metadata] = lib::decompose_instruction(*ctx.get<lib::inst_code_t>(ctx.ipx()));
			size_t offset = lib::instructions_by_code.at(opcode)->pred(ctx, args_metadata);
			ctx.ipx() += offset;
		}
	}
}
