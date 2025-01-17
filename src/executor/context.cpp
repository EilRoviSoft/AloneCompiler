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

    shared::MachineWord& Context::reg(shared::Address address) const {
        return reinterpret_cast<shared::MachineWord&>(_parent->_mframe[address]);
    }
    shared::Flags& Context::flags() const {
        return reinterpret_cast<shared::Flags&>(_parent->_mframe[FLAGS]);
    }
}
