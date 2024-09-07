#pragma once

//alone
#include "vm/info.hpp"
#include "vm/virtual_machine.hpp"

namespace alone::vm::isa {
	inline size_t halt(const context_t& ctx) {
		ctx.flags().rf = false;
		return info::inst_code_size;
	}
}
