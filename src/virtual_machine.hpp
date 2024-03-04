#pragma once
//std
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

namespace eil {
	using machine_word_t = size_t;
	using register_t = std::shared_ptr <machine_word_t>;

	enum class registers {
		asx, //Address System register
		rsx, //Result System register
		lox, //Left Operand register
		rox, //Right Operand register
		ipx, //Instruction Pointer 
		spx, //Stack Pointer
		bpx, //Base Pointer
		flags, //Flags register
		grx //General Register
	};

	const size_t machine_word_size = 8,
		registers_size = 1024,
		system_registers_size = machine_word_size * (int)registers::grx,
		general_registers_size = registers_size - system_registers_size,
		memory_size = std::pow(2, 16); //65 536

	struct flags_t {
		bool run_flag : 1; //is program running
		bool zero_flag : 1;
		bool sign_flag : 1;
		bool carry_flag : 1;
		bool overflow_flag : 1;
	};

	struct memory_address_t {
		memory_address_t() { this->val = 0; }
		memory_address_t(size_t int_val) { this->val = int_val; }
		memory_address_t(registers reg_val) { this->val = (size_t)reg_val * machine_word_size; }

		operator size_t() { return val; }

		size_t val;
	};

	memory_address_t operator+(registers lhs, size_t rhs) {
		return (size_t)lhs * machine_word_size + rhs;
	}

	struct context_t {
		machine_word_t& asx;
		machine_word_t& rsx;
		machine_word_t& lox;
		machine_word_t& rox;
		machine_word_t& ipx;
		machine_word_t& spx;
		machine_word_t& bpx;
		machine_word_t& flags;
		std::span <std::byte> mem;

		context_t(std::span <std::byte> observed_memory) : 
			mem(observed_memory),
			asx(this->get_var <machine_word_t>(registers::asx)),
			rsx(this->get_var <machine_word_t>(registers::rsx)),
			lox(this->get_var <machine_word_t>(registers::lox)),
			rox(this->get_var <machine_word_t>(registers::rox)),
			ipx(this->get_var <machine_word_t>(registers::ipx)),
			spx(this->get_var <machine_word_t>(registers::spx)),
			bpx(this->get_var <machine_word_t>(registers::bpx)),
			flags(this->get_var <machine_word_t>(registers::flags)) {
		}

		template <class _T>
		_T& get_var(memory_address_t idx) {
			return *reinterpret_cast <_T*>(&this->mem[idx]);
		}
		template <class _T>
		_T* get_arr(memory_address_t idx) {
			return reinterpret_cast <_T*>(&this->mem[idx]);
		}
	};

	using instruction_t = std::function <size_t(context_t& ctx)>;

	class virtual_machine {
	public:
		virtual_machine(size_t allocated_memory_size = memory_size) : 
			m_memory(allocated_memory_size),
			m_ctx(this->m_memory) {
		}

		void run_program(const std::vector <std::byte>& what) {
			size_t program_start_idx = this->m_memory.size() - what.size();
			for (size_t i = 0; i != what.size(); i++)
				this->m_memory[program_start_idx + i] = what[i];

			this->m_ctx.flags = true;
			while (this->m_ctx.flags) {
				size_t instruction_id = this->m_ctx.get_var <machine_word_t>(this->m_ctx.ipx);
				this->m_ctx.ipx += this->m_instructions[instruction_id](this->m_ctx);
			}
		}

		context_t& ctx() {
			return this->m_ctx;
		}

	protected:
		context_t m_ctx;
		std::vector <std::byte> m_memory;
		std::unordered_map <size_t, instruction_t> m_instructions;
	};
}