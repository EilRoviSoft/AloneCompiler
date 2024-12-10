#include "virtual_machine.hpp"

//alone
#include "instructions.hpp"
#include "util.hpp"

//isa
#include "isa/arithmetic.hpp"

namespace alone::vm {
	//VirtualMachine

	VirtualMachine::VirtualMachine() :
		ctx(),
		_p0(),
		_p1() {
	}

	void VirtualMachine::init_instruction(std::string name, inst_code_t code, size_t max_args_count, size_t bit_depth, inst_func_t pred) {
		auto ptr = std::make_shared<inst_t>(std::move(name), code, max_args_count, bit_depth, std::move(pred));
		instructions_by_name.emplace(ptr->name, ptr);
		instructions_by_code.emplace(ptr->code, ptr);
	}

	void VirtualMachine::init_instruction(inst_t&& inst) {
		auto ptr = std::make_shared<inst_t>(std::move(inst));
		instructions_by_name.emplace(ptr->name, ptr);
		instructions_by_code.emplace(ptr->code, ptr);
	}

	void VirtualMachine::init_isa() {
		for (auto& it : isa::generate_arithmetic_isa())
			init_instruction(std::move(it));
	}

	void VirtualMachine::exec(const byte_array_t& program) {
		ctx.program_size = program.size();
		for (size_t i = 0; i != ctx.program_size; ++i)
			_p0[i + registers_size] = program[i];

		ctx.init_spans(*this);
		ctx.init_registers();

		while (ctx.flags()[(size_t) flags_set::rf] && ctx.ipx() < registers_size + ctx.program_size) {
			auto [opcode, args_metadata] = decompose_instruction(*ctx.get<inst_code_t>(ctx.ipx()));
			size_t offset = instructions_by_code.at(opcode)->pred(ctx, args_metadata);
			ctx.ipx() += offset;
		}
	}
}
