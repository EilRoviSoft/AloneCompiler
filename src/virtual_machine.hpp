#pragma once
//std
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

//eil::info
#include <info.hpp>

namespace eil {
	struct memory_address_t {
		memory_address_t() { this->val = 0; }
		memory_address_t(size_t int_val) { this->val = int_val; }
		memory_address_t(info::registers reg_val) { this->val = (size_t)reg_val * info::machine_word_size; }

		operator size_t() { return val; }

		size_t val;
	};

	memory_address_t operator+(info::registers lhs, size_t rhs) {
		return (size_t)lhs * info::machine_word_size + rhs;
	}

	struct context_t {
		info::machine_word_t& asx, rsx, lox, rox, ipx, spx, bpx;
		info::flags_t& flags;
		std::span <std::byte> mem;

		context_t(std::span <std::byte> observed_memory) : 
			mem(observed_memory),
			asx(this->get_var <info::machine_word_t>(info::registers::asx)),
			rsx(this->get_var <info::machine_word_t>(info::registers::rsx)),
			lox(this->get_var <info::machine_word_t>(info::registers::lox)),
			rox(this->get_var <info::machine_word_t>(info::registers::rox)),
			ipx(this->get_var <info::machine_word_t>(info::registers::ipx)),
			spx(this->get_var <info::machine_word_t>(info::registers::spx)),
			bpx(this->get_var <info::machine_word_t>(info::registers::bpx)),
			flags(this->get_var <info::flags_t>(info::registers::flags)) {
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
		virtual_machine(size_t allocated_memory_size = info::memory_size) :
			m_memory(allocated_memory_size),
			m_ctx(this->m_memory) {
		}

		void run_program(const std::vector <std::byte>& what) {
			size_t program_start_idx = this->m_memory.size() - what.size();
			for (size_t i = 0; i != what.size(); i++)
				this->m_memory[program_start_idx + i] = what[i];

			this->m_ctx.flags.rf = true;
			while (this->m_ctx.flags.rf) {
				size_t instruction_id = this->m_ctx.get_var <info::machine_word_t>(this->m_ctx.ipx);
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