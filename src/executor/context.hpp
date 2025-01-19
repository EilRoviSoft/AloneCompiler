#pragma once

//std
#include <memory>
#include <stdexcept>

//shared
#include "shared/types.hpp"

//executor
#include "executor/util.hpp"
#include "executor/virtual_machine.hpp"

namespace amasm::executor {
    class VirtualMachine;

    class Context {
        friend class VirtualMachine;
    public:
        void set_parent(VirtualMachine& parent);

        size_t program_size() const;

        shared::machine_word& reg(shared::address address) const;
        shared::flags& flags() const;

        template<typename T = shared::machine_word>
        T* get(shared::address address) const {
            T* result;

            switch (auto [actual_address, mem_type] = decompose_address(address); mem_type) {
            case MemoryType::MainFrame:
                result = reinterpret_cast<T*>(&_parent->_mframe[actual_address]);
                break;
            case MemoryType::DynamicFrame:
                result = reinterpret_cast<T*>(_parent->_dframe.at(actual_address).data());
                break;
            default:
#ifdef DEBUG_STATUS
                throw std::runtime_error("memory type doesn't exist");
#else
                result = nullptr;
#endif
            }

            return result;
        }
        template<typename T>
        T* get_direct(shared::address address) const {
            const auto nested_address = *get(address);
            return get<T>(nested_address);
        }
        template<typename T>
        T* get_with_displacement(shared::address address) const {
            const auto nested_address = *get(address);
            const auto value = *get(nested_address);
            const auto offset = *get<ptrdiff_t>(address + shared::machine_word_size);
            return get<T>(value + offset);
        }
        // TODO: get_array

    protected:
        VirtualMachine* _parent = nullptr;

        size_t _program_size = 0;
    };
    using SharedContext = std::shared_ptr<Context>;
}
