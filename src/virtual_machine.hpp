#pragma once
//std
#include <array>
#include <cstddef>
#include <functional>
#include <span>
#include <unordered_map>

//alone::info
#include "info.hpp"

namespace alone {
	class VirtualMachine;

	template <typename T>
	using array_t = std::tuple <size_t, T*>;

	struct context_t {
		void init(VirtualMachine& vm);

		info::flags_t& flags();

		size_t program_size;
		std::span<std::byte> regs, program, stack;
		std::span<array_t<std::byte>> heap;
	};

	using inst_func_t = std::function <context_t&>;

	class VirtualMachine {
		friend context_t;
	public:
		context_t ctx;

		VirtualMachine();

		void exec(const std::vector <std::byte>& program);

		void add_instruction(info::inst_code_t id, const inst_func_t& instruction) {
			_inst_set.emplace(id, instruction);
		}
		void remove_instruction(info::inst_code_t id) {
			_inst_set.erase(id);
		}

		template <typename T>
		array_t<T> get(info::address_t address);

	private:
		std::array<std::byte, info::p0_size> _p0;
		std::array<array_t<std::byte>, info::p1_size> _p1;
		std::unordered_map<info::inst_code_t, inst_func_t> _inst_set;
	};
}
