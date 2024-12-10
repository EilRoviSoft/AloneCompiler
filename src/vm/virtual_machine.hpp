#pragma once

//std
#include <array>
#include <cstddef>

//alone
#include "general.hpp"
#include "instructions.hpp"
#include "vm_context.hpp"

namespace alone::vm {
	class VirtualMachine {
		friend context_t;
	public:
		context_t ctx;

		VirtualMachine();

		void init_instruction(std::string name, inst_code_t code, size_t max_args_count, size_t bit_depth, inst_func_t pred);
		void init_instruction(inst_t&& inst);

		void init_isa();

		void exec(const byte_array_t& program);

	private:
		std::array<std::byte, mframe_size> _p0;
		std::array<array_t<std::byte>, dframe_size> _p1;
	};
}
