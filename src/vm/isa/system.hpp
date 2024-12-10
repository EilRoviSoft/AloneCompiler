#pragma once

//alone
#include "vm_context.hpp"
#include "vm/virtual_machine.hpp"

namespace alone::vm::isa {
	inline size_t halt(const context_t& ctx) {
		ctx.flags()[(size_t) flags_set::cf] = false;
		return inst_code_size;
	}
}
