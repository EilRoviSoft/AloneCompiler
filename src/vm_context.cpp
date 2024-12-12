#include "vm_context.hpp"

//alone::vm
#include "vm/virtual_machine.hpp"

namespace alone {
	void context_t::init_spans(vm::VirtualMachine& vm) {
		mframe = std::span(vm._p0.begin(), lib::mframe_size);
		regs = std::span(vm._p0.begin(), lib::registers_size);
		program = std::span(vm._p0.begin() + lib::registers_size, program_size);
		stack = std::span(vm._p0.begin() + lib::registers_size + program_size,
		                  lib::mframe_size - lib::registers_size - program_size);
		dframe = std::span(vm._p1.begin(), lib::dframe_size);
	}

	//call it every time, when you run new program
	void context_t::init_registers() const {
		ipx() = lib::registers_size;
		spx() = lib::registers_size + program_size;
		flags()[(size_t) lib::flags_set::rf] = true;
	}

	lib::machine_word_t& context_t::asx() const {
		return reinterpret_cast<lib::machine_word_t&>(regs[(lib::machine_word_t) lib::regs_set::asx]);
	}
	lib::machine_word_t& context_t::rsx() const {
		return reinterpret_cast<lib::machine_word_t&>(regs[(lib::machine_word_t) lib::regs_set::rsx]);
	}
	lib::machine_word_t& context_t::lox() const {
		return reinterpret_cast<lib::machine_word_t&>(regs[(lib::machine_word_t) lib::regs_set::lox]);
	}
	lib::machine_word_t& context_t::rox() const {
		return reinterpret_cast<lib::machine_word_t&>(regs[(lib::machine_word_t) lib::regs_set::rox]);
	}
	lib::machine_word_t& context_t::ipx() const {
		return reinterpret_cast<lib::machine_word_t&>(regs[(lib::machine_word_t) lib::regs_set::ipx]);
	}
	lib::machine_word_t& context_t::spx() const {
		return reinterpret_cast<lib::machine_word_t&>(regs[(lib::machine_word_t) lib::regs_set::spx]);
	}
	lib::machine_word_t& context_t::bpx() const {
		return reinterpret_cast<lib::machine_word_t&>(regs[(lib::machine_word_t) lib::regs_set::bpx]);
	}

	lib::flags_t& context_t::flags() const {
		return reinterpret_cast<lib::flags_t&>(regs[(lib::machine_word_t) lib::regs_set::flags]);
	}
	lib::machine_word_t& context_t::grx(const uint64_t& id) const {
		return reinterpret_cast<lib::machine_word_t&>(regs[
			(lib::machine_word_t) lib::regs_set::grx + id * sizeof(lib::machine_word_t)]);
	}
}
