#pragma once

//library
#include "library/types.hpp"

//compiler_info
#include "compiler/info/address.hpp"
#include "compiler/info/datatype.hpp"
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
	class Variable : public IScopeElement {
		friend class VariableBuilder;
		friend class ScopeContainer;

	public:
		Variable() : IScopeElement(3) {}

		address_info address() const { return m_address_info; }
		const Datatype& datatype() const { return *m_datatype; }

		ScopeElement clone() const override {
			return std::make_shared<Variable>(*this);
		}

	protected:
		address_info m_address_info;
		const Datatype* m_datatype = nullptr;
	};
}
