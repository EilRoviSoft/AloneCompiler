#pragma once

//shared
#include "shared/types.hpp"

#define CALC_DIFF(ID) machine_word_size * ID

namespace alone::shared {
	enum class Registers : uint8_t {
		// 8-bit registers

		AL  = machine_word_size * 0,
		AH  = AL + 1,
		BL  = machine_word_size * 1,
		BH  = BL + 1,
		CL  = machine_word_size * 2,
		CH  = CL + 1,
		DL  = machine_word_size * 3,
		DH  = DL + 1,

		// 16-bit registers

		AX    = AL,
		BX    = BL,
		CX    = CL,
		DX    = DL,
		IP    = machine_word_size * 4,
		BP    = machine_word_size * 5,
		CP    = machine_word_size * 6,
		SP    = machine_word_size * 7,
		FLAGS = machine_word_size * 8,
		GP    = machine_word_size * 10,

		// 32-bit registers

		EAX    = AL,
		EBX    = BL,
		ECX    = CL,
		EDX    = DL,
		EIP    = IP,
		EBP    = BP,
		ECP    = CP,
		ESP    = SP,
		EFLAGS = FLAGS,
		EGP    = GP,

		// 64-bit registers

		RAX    = AL,
		RBX    = BL,
		RCX    = CL,
		RDX    = DL,
		RIP    = IP,
		RBP    = BP,
		RCP    = CP,
		RSP    = SP,
		RFLAGS = FLAGS,
		RGP    = GP
	};
}
