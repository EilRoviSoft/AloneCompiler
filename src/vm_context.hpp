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
        std::span<lib::array_t<std::byte>> dframe;

        void init_spans(vm::VirtualMachine& vm);
        void init_registers() const;

        lib::machine_word_t& asx() const;
        lib::machine_word_t& rsx() const;
        lib::machine_word_t& lox() const;
        lib::machine_word_t& rox() const;
        lib::machine_word_t& ipx() const;
        lib::machine_word_t& spx() const;
        lib::machine_word_t& bpx() const;
        lib::flags_t& flags() const;
        lib::machine_word_t& grx(const uint64_t& id) const;

        template<class T = lib::machine_word_t>
        T* get(const lib::address_t& address) const {
            T* result;

            switch (auto [actual_address, mem_type] = lib::decompose_address(address); mem_type) {
            case lib::memory_type::mframe:
                result = reinterpret_cast<T*>(&mframe[actual_address]);
                break;
            case lib::memory_type::dframe:
                result = reinterpret_cast<T*>(dframe[actual_address].ptr);
                break;
            default:
                throw std::runtime_error("virtual_machine.cpp: This memory type doesn't exist.");
            }

            return result;
        }
        template<class T = lib::machine_word_t>
        T* get_with_displacement(const lib::address_t& address) const {
            return get<T>(*get(address) + *get(address + lib::machine_word_size));
        }
        template<class T>
        lib::array_t<T> get_array(const lib::address_t& address) const {
            lib::array_t<T> result;

            auto [actual_address, mem_type] = lib::decompose_address(address);
            switch (mem_type) {
            case lib::memory_type::mframe:
                result = {
                    .size = sizeof(T),
                    .ptr = reinterpret_cast<T*>(&mframe[actual_address])
                };
                break;
            case lib::memory_type::dframe:
                result = dframe[actual_address];
                break;
            default:
                throw std::runtime_error("virtual_machine.cpp: This memory type doesn't exist.");
            }

            return result;
        }
    };
}
