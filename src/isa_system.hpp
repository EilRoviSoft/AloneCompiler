#pragma once
//alone
#include "info.hpp"
#include "virtual_machine.hpp"

namespace alone::isa {
	inline size_t halt(context_t& ctx) {
		ctx.flags().rf = false;
		return info::inst_code_size;
	}
}
