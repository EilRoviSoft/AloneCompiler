#pragma once

//shared
#include "shared/bytecode.hpp"

//compiler
#include "compiler/info/context.hpp"
#include "compiler/info/functions.hpp"

namespace amasm::compiler {
	class Translator {
	public:
		struct hint {
			size_t offset;
			std::string name;
		};

		explicit Translator(Context& ctx);

		shared::Bytecode translate(funcs_queue composed) const;

	private:
		Context& _ctx;

		std::tuple<shared::Bytecode, std::list<hint>> _decompose_instruction(const func_info& scope,
		                                                                     const inst_decl& inst)
		const;
	};
}
