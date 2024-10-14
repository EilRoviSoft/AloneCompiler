#include "virtual_machine.hpp"

//std
#include <ranges>
#include <stdexcept>

//alone
#include "util.hpp"

namespace alone::vm {
	//VirtualMachine

	VirtualMachine::VirtualMachine() :
		ctx(),
		_p0(),
		_p1() {
	}

	void VirtualMachine::init_isa() {}

	void VirtualMachine::exec(const byte_array_t& program) {
		ctx.program_size = program.size();
		for (size_t i = 0; i != ctx.program_size; ++i)
			_p0[i + registers_size] = program[i];

		ctx.init_spans(*this);
		ctx.init_registers();

		while (ctx.flags()[(size_t) flags_set::rf] && ctx.ipx() < registers_size + ctx.program_size) {
			inst_code_t opcode = *get<inst_code_t>(ctx.ipx());
			size_t offset = instructions_by_code.at(opcode)->func(ctx);
			ctx.ipx() += offset;
		}
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
