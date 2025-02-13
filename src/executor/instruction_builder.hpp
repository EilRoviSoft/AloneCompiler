#pragma once

//library
#include "library/builder.hpp"

//executor
#include "executor/instruction.hpp"

namespace amasm::executor {
	class InstructionBuilder : public lib::IBuilder<Instruction> {
	public:
		InstructionBuilder& set_id(lib::inst_code id);
		InstructionBuilder& set_predicate(InstSignature predicate);
		InstructionBuilder& set_memory_shift(ptrdiff_t memory_shift);

		bool is_built() const override;

	private:
		struct {
			bool id        : 1 = false;
			bool predicate : 1 = false;
		} _is_set;

		const char* get_error_message() const override;
	};
}
