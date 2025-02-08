#pragma once

//library
#include "library/builder.hpp"

//executor
#include "executor/instruction.hpp"

namespace amasm::executor {
	class InstructionBuilder : public lib::IBuilder<Instruction> {
	public:
		InstructionBuilder& set_id(lib::inst_code id);
		InstructionBuilder& set_memory_shift(ptrdiff_t memory_shift);
		InstructionBuilder& set_predicate(Instruction::Signature predicate);

		Instruction&& get_product() override;

	private:
		struct {
			bool id        : 1 = false;
			bool predicate : 1 = false;
		} _is_set;
	};
}
