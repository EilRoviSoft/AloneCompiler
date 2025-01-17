#include "context.hpp"

//shared
#include "shared/types.hpp"

//executor
#include "executor/util.hpp"
#include "executor/virtual_machine.hpp"

namespace amasm::executor {
    void Context::set_parent(VirtualMachine& parent) {
        _parent = &parent;
    }

    size_t Context::program_size() const {
        return _program_size;
    }

    shared::machine_word& Context::reg(shared::address address) const {
        return reinterpret_cast<shared::machine_word&>(_parent->_mframe[address]);
    }
    shared::flags& Context::flags() const {
        return reinterpret_cast<shared::flags&>(_parent->_mframe[FLAGS]);
    }
}
