#pragma once
//std
#include <cstdint>

namespace alone::info {
	enum class registers {
		asx = 0x00, //Address System register
		rsx = 0x08, //Result System register
		lox = 0x10, //Left Operand register
		rox = 0x18, //Right Operand register
		ipx = 0x20, //Instruction Pointer 
		spx = 0x28, //Stack Pointer
		bpx = 0x30, //Base Pointer
		flx = 0x38, //Flags register
		grx = 0x40, //General Registers start
	};
	enum class memory_type {
		mframe, //registers + application + stack
		dframe, //pointers
		eframe, //external memory
		sframe, //static memory
	};

	struct flags_t {
		bool rf : 1; //is program running
		bool zf : 1; //zero flag
		bool sf : 1; //sign flag
		bool cf : 1; //carry flag
		bool of : 1; //overflow flag
	};

	using address_t = uint64_t;
	using mword_t = uint64_t;
	using inst_code_t = uint16_t;

	constexpr size_t machine_word_size = sizeof(uint64_t),
		inst_code_size = sizeof(inst_code_t),
		registers_size = 256,
		system_registers_size = machine_word_size * (int) registers::grx,
		general_registers_size = registers_size - system_registers_size,
		p0_size = 2 << 16,
		p1_size = 16;
}