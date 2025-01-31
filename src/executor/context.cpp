#include "context.hpp"

//library
#include "library/types.hpp"

//executor
#include "executor/virtual_machine.hpp"

namespace amasm::executor {
    Context::Context(VirtualMachine& vm) :
        _vm(vm) {
    }

    size_t Context::program_size() const {
        return _program_size;
    }
    lib::machine_word& Context::operator[](lib::address address) const {
        return *reinterpret_cast<lib::machine_word*>(&_vm._mframe[address]);
    }
    std::bitset<64>::reference Context::operator[](FlagType flag_id) const {
        return reinterpret_cast<lib::flags&>(_vm._mframe[FLAGS])[flag_id];
    }
}
