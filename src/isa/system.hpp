#pragma once
//alone::alone
#include <virtual_machine.hpp>

//alone::isa
#include <info.hpp>

namespace alone::isa {
	size_t halt(context_t& ctx) {
		ctx.flags.rf = false;
		return info::inst_size;
	}
}