#pragma once

//std
#include <span>

//alone
#include "general.hpp"

namespace alone::vm {
	class VirtualMachine;
}

namespace alone {
	struct context_t {
		size_t program_size;
		std::span<std::byte> regs, program, stack;
		std::span<array_t<std::byte>> heap;

		void init_spans(vm::VirtualMachine& vm);
		void init_registers() const;

		machine_word_t& asx() const;
		machine_word_t& rsx() const;
		machine_word_t& lox() const;
		machine_word_t& rox() const;
		machine_word_t& ipx() const;
		machine_word_t& spx() const;
		machine_word_t& bpx() const;
		flags_t& flags() const;
		machine_word_t& grx(uint64_t id) const;
	};
}