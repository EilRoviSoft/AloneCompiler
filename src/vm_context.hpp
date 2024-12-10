#pragma once

//std
#include <span>
#include <stdexcept>

//alone
#include "general.hpp"

namespace alone::vm {
	class VirtualMachine;
}

namespace alone {
	struct context_t {
		size_t program_size;
		std::span<std::byte> mframe, regs, program, stack;
		std::span<array_t<std::byte>> dframe;

		void init_spans(vm::VirtualMachine& vm);
		void init_registers() const;

		machine_word_t& asx() const;
		machine_word_t& rsx() const;
		machine_word_t& lox() const;
		machine_word_t& rox() const;
		machine_word_t& ipx() const;
		machine_word_t& spx() const;
		machine_word_t& bpx() const;
		flags_t& flags() const;
		machine_word_t& grx(const uint64_t& id) const;

		template<class T = machine_word_t>
		T* get(const address_t& address) const {
			T* result;

			auto [actual_address, mem_type] = decompose_address(address);
			if (mem_type == memory_type::mframe) {
				result = reinterpret_cast<T*>(&mframe[actual_address]);
			} else if (mem_type == memory_type::dframe) {
				auto [size, ptr] = dframe[actual_address];
				result = reinterpret_cast<T*>(ptr);
			} else
				throw std::runtime_error("virtual_machine.cpp: This memory type doesn't exist.");

			return result;
		}
		template<class T = machine_word_t>
		T* get_with_displacement(const address_t& address) const {
			const auto& start = *get(address);
			const auto& offset = *get(address + machine_word_size);
			return get<T>(start + offset);
		}
		template<class T>
		array_t<T> get_array(const address_t& address) const {
			array_t<T> result;

			auto [actual_address, mem_type] = decompose_address(address);
			switch (mem_type) {
			case memory_type::mframe:
				result = {
					.size = sizeof(T),
					.ptr = reinterpret_cast<T*>(&mframe[actual_address])
				};
				break;
			case memory_type::dframe:
				result = dframe[actual_address];
				break;
			default:
				throw std::runtime_error("virtual_machine.cpp: This memory type doesn't exist.");
			}

			return result;
		}
	};
}
