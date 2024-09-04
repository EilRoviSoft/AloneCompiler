#include "virtual_machine.hpp"
//std
#include <exception>
//alone::util
#include "util.hpp"

namespace alone {
	//context_t

	void context_t::init(VirtualMachine& vm) {
		regs = std::span(vm._p0.begin(), info::registers_size);
		program = std::span(vm._p0.begin() + info::registers_size, program_size);
		stack = std::span(vm._p0.begin() + info::registers_size + program_size, info::p0_size - info::registers_size - program_size);
		heap = std::span(vm._p1.begin(), info::p1_size);
	}

	info::flags_t& context_t::flags() { return *reinterpret_cast<info::flags_t*>(regs[(size_t) info::registers::flx]); }

	//VirtualMachine

	VirtualMachine::VirtualMachine() : ctx(), _p0() {}

	void VirtualMachine::exec(const std::vector <std::byte>& program) {
		ctx.program_size = program.size();
		ctx.init(*this);
		for (size_t i = 0; i != ctx.program_size; ++i)
			_p0[i + info::registers_size] = program[i];

		while (ctx.flags().rf) {
			
		}
	}

	template <class T>
	array_t<T> VirtualMachine::get(info::address_t address) {
		array_t<T> result;

		auto [mem_type, actual_address] = util::decompose(address);
		switch (mem_type) {
		case info::memory_type::mframe:
			result = { sizeof(T), reinterpret_cast<T*>(&_p0[actual_address]) };
			break;
		case info::memory_type::dframe:
			result = _p1[actual_address];
			break;
		default:
			throw(std::exception("This memory type doesn't exist."));
		}

		return result;
	}
}
