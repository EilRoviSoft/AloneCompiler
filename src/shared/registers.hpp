#pragma once

//shared
#include "shared/types.hpp"

#define CALC_DIFF(ID) machine_word_size * ID

namespace alone::shared {
	enum class Registers : uint8_t {
		AL     = machine_word_size * 0,
		AH     = AL + 1,
		BL     = machine_word_size * 1,
		BH     = BL + 1,
		CL     = machine_word_size * 2,
		CH     = CL + 1,
		DL     = machine_word_size * 3,
		DH     = DL + 1,
		IPL    = machine_word_size * 4,
		BPL    = machine_word_size * 5,
		CPL    = machine_word_size * 6,
		SPL    = machine_word_size * 7,
		LFLAGS = machine_word_size * 8,
		GPL    = machine_word_size * 10,

#if SYSTEM_BIT_DEPTH >= 16
		AX    = AL,
		BX    = BL,
		CX    = CL,
		DX    = DL,
		IP    = IPL,
		BP    = BPL,
		CP    = CPL,
		SP    = SPL,
		FLAGS = LFLAGS,
		GP    = GPL,
#endif

#if SYSTEM_BIT_DEPTH >= 32
		EAX    = AL,
		EBX    = BL,
		ECX    = CL,
		EDX    = DL,
		EIP    = IPL,
		EBP    = BPL,
		ECP    = CPL,
		ESP    = SPL,
		EFLAGS = LFLAGS,
		EGP    = GPL,
#endif

#if SYSTEM_BIT_DEPTH >= 64
		RAX    = AL,
		RBX    = BL,
		RCX    = CL,
		RDX    = DL,
		RIP    = IPL,
		RBP    = BPL,
		RCP    = CPL,
		RSP    = SPL,
		RFLAGS = LFLAGS,
		RGP    = GPL
#endif
	};
}
