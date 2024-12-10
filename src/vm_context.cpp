#include "vm_context.hpp"

//alone
#include "vm/virtual_machine.hpp"

namespace alone {
	void context_t::init_spans(vm::VirtualMachine& vm) {
		mframe = std::span(vm._p0.begin(), mframe_size);
		regs = std::span(vm._p0.begin(), registers_size);
		program = std::span(vm._p0.begin() + registers_size, program_size);
		stack = std::span(vm._p0.begin() + registers_size + program_size, mframe_size - registers_size - program_size);
		dframe = std::span(vm._p1.begin(), dframe_size);
	}

	//call it every time, when you run new program
	void context_t::init_registers() const {
		ipx() = registers_size;
		spx() = registers_size + program_size;
		flags()[(size_t) flags_set::rf] = true;
	}

	machine_word_t& context_t::asx() const {
		return reinterpret_cast<machine_word_t&>(regs[(machine_word_t) regs_set::asx]);
	}
	machine_word_t& context_t::rsx() const {
		return reinterpret_cast<machine_word_t&>(regs[(machine_word_t) regs_set::rsx]);
	}
	machine_word_t& context_t::lox() const {
		return reinterpret_cast<machine_word_t&>(regs[(machine_word_t) regs_set::lox]);
	}
	machine_word_t& context_t::rox() const {
		return reinterpret_cast<machine_word_t&>(regs[(machine_word_t) regs_set::rox]);
	}
	machine_word_t& context_t::ipx() const {
		return reinterpret_cast<machine_word_t&>(regs[(machine_word_t) regs_set::ipx]);
	}
	machine_word_t& context_t::spx() const {
		return reinterpret_cast<machine_word_t&>(regs[(machine_word_t) regs_set::spx]);
	}
	machine_word_t& context_t::bpx() const {
		return reinterpret_cast<machine_word_t&>(regs[(machine_word_t) regs_set::bpx]);
	}
	flags_t& context_t::flags() const {
		return reinterpret_cast<flags_t&>(regs[(machine_word_t) regs_set::flags]);
	}
	machine_word_t& context_t::grx(const uint64_t& id) const {
		return reinterpret_cast<machine_word_t&>(regs[(machine_word_t) regs_set::grx + id * sizeof(machine_word_t)]);
	}
}
