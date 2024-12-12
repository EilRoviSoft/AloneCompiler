#pragma once

//alone
#include "vm_context.hpp"

//alone::vm
#include "vm/virtual_machine.hpp"

namespace alone::vm::isa {
    inline size_t halt(const context_t& ctx) {
        ctx.flags()[(size_t) lib::flags_set::cf] = false;
        return lib::inst_code_size;
    }
}
