#pragma once

//std
#include <memory>
#include <span>
#include <stdexcept>

//library
#include "library/types.hpp"

//executor
#include "executor/util.hpp"
#include "executor/virtual_machine.hpp"

namespace amasm::executor {
    class VirtualMachine;

    class Context {
        friend class VirtualMachine;

    public:
        explicit Context(VirtualMachine& vm);

        size_t program_size() const;

        // same as get(address) to get variable from mframe, but faster, because it doesn't do additional checks
        // also always returns machine_word instead of T
        // primarily used for getting registers
        lib::machine_word& operator[](lib::address address) const;
        std::bitset<64>::reference operator[](FlagType flag_id) const;

        template<typename T = lib::machine_word>
        T* get(lib::address address) const {
            T* result;

            switch (auto [actual_address, mem_type] = decompose_address(address); mem_type) {
            case MemoryType::MainFrame:
                result = reinterpret_cast<T*>(&_vm._mframe[actual_address]);
                break;
            case MemoryType::DynamicFrame:
                result = reinterpret_cast<T*>(_vm._dframe.at(actual_address).data());
                break;
            default:
                throw std::runtime_error("memory type doesn't exist");
            }

            return result;
        }
        template<typename T>
        T* get_direct(lib::address address) const {
            const auto nested_address = *get(address);
            return get<T>(nested_address);
        }
        template<typename T>
        T* get_with_displacement(lib::address address) const {
            const auto nested_address = *get(address);
            const auto value = *get(nested_address);
            const auto offset = *get<ptrdiff_t>(address + lib::machine_word_size);
            return get<T>(value + offset);
        }
        // TODO: get_array

        void native_call(lib::address address) const {
            (*this)[IP] = true;
            _vm._natives.at(address)(*this);
            (*this)[IP] = false;
        }

    protected:
        VirtualMachine& _vm;

        size_t _program_size = 0;
    };
    using SharedContext = std::shared_ptr<Context>;
}

namespace amasm {
    using ExecutorContext = executor::Context;
}
