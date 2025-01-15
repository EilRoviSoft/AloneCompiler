#pragma once

//shared
#include "shared/bytecode.hpp"

//amasm
#include "amasm/info/context.hpp"
#include "amasm/info/functions.hpp"

namespace alone::amasm {
	class Translator {
	public:
		explicit Translator(Context& ctx);

		shared::Bytecode translate(composed_funcs_t composed) const;

	private:
		Context& _ctx;
	};
}
