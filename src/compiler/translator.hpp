#pragma once

//library
#include "library/bytecode.hpp"

//compiler_info
#include "compiler/info/scope_container.hpp"

namespace amasm::compiler {
	class Translator {
	public:
		static lib::Bytecode translate(const ScopeContainer& container);
	};
}
