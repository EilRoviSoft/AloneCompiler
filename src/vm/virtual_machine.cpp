#include "virtual_machine.hpp"

//std
#include <ranges>
#include <stdexcept>

//alone::vm::util
#include "vm/util.hpp"

namespace alone::vm {
	//context_t

	void context_t::init_spans(VirtualMachine& vm) {
		regs = std::span(vm._p0.begin(), registers_size);
		program = std::span(vm._p0.begin() + registers_size, program_size);
		stack = std::span(vm._p0.begin() + registers_size + program_size, mframe_size - registers_size - program_size);
		heap = std::span(vm._p1.begin(), dframe_size);
	}

	//call it every time, when you run new program
	void context_t::init_registers() const {
		ipx() = registers_size;
		spx() = registers_size + program_size;
		flags()[static_cast<unsigned long long>(flags_set::rf)] = true;
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
	machine_word_t& context_t::grx(uint64_t id) const {
		return reinterpret_cast<machine_word_t&>(regs[(machine_word_t) regs_set::grx + id * sizeof(machine_word_t)]);
	}

	//VirtualMachine

	VirtualMachine::VirtualMachine() :
		ctx(),
		_p0(),
		_p1() {}

	void VirtualMachine::init_isa() {}

	void VirtualMachine::exec(const byte_array_t& program) {
		ctx.program_size = program.size();
		for (size_t i = 0; i != ctx.program_size; ++i)
			_p0[i + registers_size] = program[i];

		ctx.init_spans(*this);
		ctx.init_registers();

		while (ctx.flags()[(size_t) flags_set::rf] && ctx.ipx() < registers_size + ctx.program_size) {
			inst_code_t opcode = *get<inst_code_t>(ctx.ipx());
			size_t offset = _inst_set[opcode](ctx);
			ctx.ipx() += offset;
		}
	}

	void VirtualMachine::add_instruction(inst_code_t id, const inst_func_t& instruction) {
		_inst_set.emplace(id, instruction);
	}

	void VirtualMachine::remove_instruction(inst_code_t id) {
		_inst_set.erase(id);
	}

	template<class T> T* VirtualMachine::get(address_t address) {
		T* result;

		auto [actual_address, mem_type] = decompose_address(address);
		if (mem_type == memory_type::mframe) {
			result = reinterpret_cast<T*>(&_p0[actual_address]);
		} else if (mem_type == memory_type::dframe) {
			auto [size, ptr] = _p1[actual_address];
			result = reinterpret_cast<T*>(ptr);
		} else
			throw std::runtime_error("virtual_machine.cpp: This memory type doesn'type exist.");

		return result;
	}
	template<class T> array_t<T> VirtualMachine::get_array(address_t address) {
		array_t<T> result;

		auto [actual_address, mem_type] = decompose_address(address);
		switch (mem_type) {
		case memory_type::mframe:
			result = { .size = sizeof(T), .ptr = reinterpret_cast<T*>(&_p0[actual_address]) };
			break;
		case memory_type::dframe:
			result = _p1[actual_address];
			break;
		default:
			throw std::runtime_error("virtual_machine.cpp: This memory type doesn'type exist.");
		}

		return result;
	}
}
