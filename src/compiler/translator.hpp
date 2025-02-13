#pragma once

//std
#include <list>
#include <string>
#include <tuple>

//library
#include "library/bytecode.hpp"

//compiler_info
#include "compiler/info/scope_container.hpp"

namespace amasm::compiler {
	class Translator {
		struct hint {
			std::string name;
			size_t offset;
		};
	public:
		lib::Bytecode translate(const ScopeContainer& container);

	private:
		ConstScopeProxy _scopes;

		std::tuple<lib::Bytecode, std::list<hint>> _generate_inst_bytecode(const InstDecl& inst, size_t scope_id);
	};
}
