#pragma once

//std
#include <array>
#include <functional>

//alone
#include "error_codes.hpp"
#include "instructions.hpp"
#include "vm_context.hpp"

//alone::isa
#include "isa/universal.hpp"

namespace alone::isa {
	template<typename T>
	void inst_mov(const context_t& ctx, const std::array<argument_type, 4>& args_metadata) {

	}
}
