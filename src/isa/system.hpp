#pragma once
//eil::vm
#include <virtual_machine.hpp>

//eil::isa
#include <info.hpp>

namespace eil::isa {
	size_t halt(context_t& ctx) {
		ctx.flags.rf = false;
		return info::inst_size;
	}
}