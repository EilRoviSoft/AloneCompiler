#pragma once

//library
#include "library/types.hpp"

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
	class Variable : public IScopeElement {
		friend class VariableBuilder;

	public:
		Variable();

		lib::address address() const { return m_address; }
		const Datatype& datatype() const { return *m_datatype; }

	protected:
		lib::address m_address;
		const Datatype* m_datatype;
	};
}
