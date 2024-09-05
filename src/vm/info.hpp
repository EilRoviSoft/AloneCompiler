#pragma once
//std
#include <cstdint>
#include <functional>
#include <tuple>

namespace alone::vm {
	template <typename T>
	using array_t = std::tuple <size_t, T*>;

	using address_t = uint64_t;
	using machine_word_t = uint64_t;
	using inst_code_t = uint16_t;
	using inst_func_t = std::function <size_t(const struct context_t&)>;

	struct flags_t {
		bool rf : 1; //is program running
		bool zf : 1; //zero flag
		bool sf : 1; //sign flag
		bool cf : 1; //carry flag
		bool of : 1; //overflow flag
		size_t unspecified : 59;
	};
}

namespace alone::vm::info {
	enum registers {
		asx = 0 * sizeof(machine_word_t), //Address System register
		rsx = 1 * sizeof(machine_word_t), //Result System register
		lox = 2 * sizeof(machine_word_t), //Left Operand register
		rox = 3 * sizeof(machine_word_t), //Right Operand register
		ipx = 4 * sizeof(machine_word_t), //Instruction Pointer
		spx = 5 * sizeof(machine_word_t), //Stack Pointer
		bpx = 6 * sizeof(machine_word_t), //Base Pointer
		flags = 7 * sizeof(machine_word_t), //Flags register
		grx = 8 * sizeof(machine_word_t), //General Registers start
	};
	enum memory_type {
		mframe, //registers + application + stack
		dframe, //pointers
		eframe, //external memory
		sframe, //static memory
	};

	constexpr size_t machine_word_size = sizeof(uint64_t),
		inst_code_size = sizeof(inst_code_t),
		registers_size = 256,
		system_registers_size = machine_word_size * (int) registers::grx,
		general_registers_size = registers_size - system_registers_size,
		p0_size = 2 << 16,
		p1_size = 16;
}