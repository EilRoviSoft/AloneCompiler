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
		Variable() : IScopeElement(3) {}

		lib::address address() const { return m_address; }
		const Datatype& datatype() const { return *m_datatype; }

		ScopeElement clone() const override {
			return std::make_shared<IScopeElement>(*this);
		}

	protected:
		lib::address m_address = 0;
		const Datatype* m_datatype = nullptr;
	};
}
