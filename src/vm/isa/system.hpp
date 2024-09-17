#pragma once

//alone
#include "vm/info.hpp"
#include "vm/virtual_machine.hpp"

namespace alone::vm::isa {
	inline size_t halt(const context_t& ctx) {
		ctx.flags()[flags_set::cf] = false;
		return inst_code_size;
	}
}
