#pragma once

//std
#include <bitset>
#include <cstdint>
#include <functional>

namespace alone::vm {
	using address_t = uint64_t;
	using machine_word_t = uint64_t;
	using inst_code_t = uint32_t;
	using inst_func_t = std::function<size_t(const struct context_t&)>;
	using flags_t = std::bitset<64>;

	enum class regs_set : size_t {
		asx   = 0 * sizeof(machine_word_t), //Address System register
		rsx   = 1 * sizeof(machine_word_t), //Result System register
		lox   = 2 * sizeof(machine_word_t), //Left Operand register
		rox   = 3 * sizeof(machine_word_t), //Right Operand register
		ipx   = 4 * sizeof(machine_word_t), //Instruction Pointer
		bpx   = 5 * sizeof(machine_word_t), //Stack Pointer
		spx   = 6 * sizeof(machine_word_t), //Base Pointer
		flags = 7 * sizeof(machine_word_t), //Flags register
		grx   = 8 * sizeof(machine_word_t), //General Registers start
	};
	enum class flags_set : size_t {
		rf = 0, //is program running flag
		zf = 1, //zero flag
		sf = 2, //sign flag
		cf = 3, //carry flag
		of = 4, //overflow flag
	};
	enum class memory_type : size_t {
		mframe, //registers + application + stack
		dframe, //pointers
		eframe, //external memory
		sframe, //static memory
	};
	enum class argument_type : size_t {
		empty = 0,
		memory,
		immediate,
		indirect_with_displacement,
	};

	constexpr size_t machine_word_size = sizeof(uint64_t);
	constexpr size_t inst_code_size = sizeof(inst_code_t);
	constexpr size_t registers_size = 256;
	constexpr size_t system_registers_size = registers_size - static_cast<int>(regs_set::grx);
	constexpr size_t general_registers_size = registers_size - system_registers_size;
	constexpr size_t mframe_size = 1 << 16;
	constexpr size_t dframe_size = 16;
}
