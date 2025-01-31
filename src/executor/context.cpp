#include "context.hpp"

//library
#include "library/types.hpp"

//executor
#include "executor/virtual_machine.hpp"

namespace amasm::executor {
    void Context::set_parent(VirtualMachine& parent) {
        _parent = &parent;
    }

    size_t Context::program_size() const {
        return _program_size;
    }

    lib::machine_word& Context::reg(lib::address address) const {
        return reinterpret_cast<lib::machine_word&>(_parent->_mframe[address]);
    }
    lib::flags& Context::flags() const {
        return reinterpret_cast<lib::flags&>(_parent->_mframe[FLAGS]);
    }
}
