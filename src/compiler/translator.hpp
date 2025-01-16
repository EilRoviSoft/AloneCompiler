#pragma once

//shared
#include "shared/bytecode.hpp"

//compiler
#include "compiler/info/context.hpp"
#include "compiler/info/functions.hpp"

namespace amasm::compiler {
	class Translator {
	public:
		struct hint_t {
			size_t offset;
			std::string name;
		};

		explicit Translator(Context& ctx);

		shared::Bytecode translate(composed_funcs_t composed) const;

	private:
		Context& _ctx;

		std::tuple<shared::Bytecode, std::list<hint_t>> _decompose_instruction(const func_info_t& scope,
		                                                                       const inst_decl_t& inst) const;
	};
}
