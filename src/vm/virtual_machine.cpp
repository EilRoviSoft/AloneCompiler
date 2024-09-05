#include "virtual_machine.hpp"
//std
#include <exception>

//alone::vm::util
#include "vm/util.hpp"

namespace alone::vm {
	//context_t

	void context_t::init_spans(VirtualMachine& vm) {
		regs = std::span(vm._p0.begin(), info::registers_size);
		program = std::span(vm._p0.begin() + info::registers_size, program_size);
		stack = std::span(vm._p0.begin() + info::registers_size + program_size, info::mframe_size - info::registers_size - program_size);
		heap = std::span(vm._p1.begin(), info::dframe_size);
	}

	//call it every time, when you run new program
	void context_t::init_registers() const {
		ipx() = info::registers_size;
		spx() = info::registers_size + program_size;
		flags().rf = true;
	}

	machine_word_t& context_t::asx() const {
		return reinterpret_cast<machine_word_t&>(regs[info::asx]);
	}
	machine_word_t& context_t::rsx() const {
		return reinterpret_cast<machine_word_t&>(regs[info::rsx]);
	}
	machine_word_t& context_t::lox() const {
		return reinterpret_cast<machine_word_t&>(regs[info::lox]);
	}
	machine_word_t& context_t::rox() const {
		return reinterpret_cast<machine_word_t&>(regs[info::rox]);
	}
	machine_word_t& context_t::ipx() const {
		return reinterpret_cast<machine_word_t&>(regs[info::ipx]);
	}
	machine_word_t& context_t::spx() const {
		return reinterpret_cast<machine_word_t&>(regs[info::spx]);
	}
	machine_word_t& context_t::bpx() const {
		return reinterpret_cast<machine_word_t&>(regs[info::bpx]);
	}
	flags_t& context_t::flags() const {
		return reinterpret_cast<flags_t&>(regs[info::flags]);
	}
	machine_word_t& context_t::grx(uint64_t id) const {
		return reinterpret_cast<machine_word_t&>(regs[info::grx + id * sizeof(machine_word_t)]);
	}

	//VirtualMachine

	VirtualMachine::VirtualMachine() : ctx(), _p0() {}

	void VirtualMachine::init_isa() {
		
	}

	void VirtualMachine::exec(const std::vector <std::byte>& program) {
		ctx.program_size = program.size();
		for (size_t i = 0; i != ctx.program_size; ++i)
			_p0[i + info::registers_size] = program[i];

		ctx.init_spans(*this);
		ctx.init_registers();

		while (ctx.flags().rf && ctx.ipx() < info::registers_size + ctx.program_size) {
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

	template <class T>
	T* VirtualMachine::get(address_t address) {
		T* result;

		auto [mem_type, actual_address] = util::decompose(address);
		switch (mem_type) {
		case info::mframe:
			result = reinterpret_cast<T*>(&_p0[actual_address]);
			break;
		case info::dframe:
			auto [size, ptr] = _p1[actual_address];
			result = reinterpret_cast<T*>(ptr);
			break;
		default:
			throw(std::exception("This memory type doesn't exist."));
		}

		return result;
	}
	template <class T>
	array_t<T> VirtualMachine::get_array(address_t address) {
		array_t<T> result;

		auto [mem_type, actual_address] = util::decompose(address);
		switch (mem_type) {
		case info::mframe:
			result = { sizeof(T), reinterpret_cast<T*>(&_p0[actual_address]) };
			break;
		case info::dframe:
			result = _p1[actual_address];
			break;
		default:
			throw(std::exception("This memory type doesn't exist."));
		}

		return result;
	}
}
