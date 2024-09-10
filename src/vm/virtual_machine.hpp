#pragma once

//std
#include <array>
#include <cstddef>
#include <span>
#include <unordered_map>

//alone
#include "general.hpp"

//alone::vm::info
#include "vm/info.hpp"

namespace alone::vm {
	struct context_t {
		size_t program_size;
		std::span<std::byte> regs, program, stack;
		std::span<array_t<std::byte>> heap;

		void init_spans(class VirtualMachine& vm);
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

	class VirtualMachine {
		friend context_t;
	public:
		context_t ctx;

		VirtualMachine();

		void init_isa();
		void exec(const std::vector <std::byte>& program);

		void add_instruction(inst_code_t id, const inst_func_t& instruction);
		void remove_instruction(inst_code_t id);

		template<class T>
		T* get(address_t address);
		template<class T>
		array_t<T> get_array(address_t address);

	private:
		std::array<std::byte, info::mframe_size> _p0;
		std::array<array_t<std::byte>, info::dframe_size> _p1;
		std::unordered_map<inst_code_t, inst_func_t> _inst_set;
	};
}
