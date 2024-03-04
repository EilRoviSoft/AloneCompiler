#pragma once
//eil::vm
#include <virtual_machine.hpp>

namespace eil::isa {
	size_t halt(context_t& ctx) {
		ctx.get_flags().run_flag = false;
	}
}