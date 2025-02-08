#pragma once

//library
#include "library/bytecode.hpp"

//compiler_info
#include "compiler/info/scope_container.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler {
	class Translator {
	public:
		explicit Translator(Context& ctx);

		lib::Bytecode translate(ScopeContainer container) const;

	private:
		Context& _ctx;
	};
}
