#pragma once

//std
#include <array>
#include <cstddef>
#include <unordered_map>

//alone
#include "general.hpp"
#include "instructions.hpp"
#include "vm_context.hpp"

namespace alone::vm {
	class VirtualMachine {
		friend context_t;
	public:
		context_t ctx;

		VirtualMachine();

		void init_isa();
		void exec(const byte_array_t& program);

		template<class T>
		T* get(address_t address);
		template<class T>
		array_t<T> get_array(address_t address);

	private:
		std::array<std::byte, mframe_size> _p0;
		std::array<array_t<std::byte>, dframe_size> _p1;
	};
}
