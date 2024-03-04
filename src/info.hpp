#pragma once
//std
#include <cstdint>

namespace eil::info {
	using machine_word_t = size_t;

	enum class registers {
		asx, //Address System register
		rsx, //Result System register
		lox, //Left Operand register
		rox, //Right Operand register
		ipx, //Instruction Pointer 
		spx, //Stack Pointer
		bpx, //Base Pointer
		flags, //Flags register
		grx //General Register
	};

	struct flags_t {
		bool rf : 1; //is program running
		bool zf : 1; //zero flag
		bool sf : 1; //sign flag
		bool cf : 1; //carry flag
		bool of : 1; //overflow flag
	};

	const size_t machine_word_size = 8,
		inst_size = sizeof(uint16_t),
		registers_size = 1024,
		system_registers_size = machine_word_size * (int)registers::grx,
		general_registers_size = registers_size - system_registers_size,
		memory_size = 2 << 16;
}