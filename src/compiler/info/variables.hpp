#pragma once

//library
#include "library/types.hpp"
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/datatypes.hpp"
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
	class Variable : public IScopeElement {
		friend class Scope;
		friend class VariableBuilder;

	public:
		Variable();

		lib::address address() const;
		const Datatype& datatype() const;

	protected:
		lib::address m_address;
		const Datatype* m_datatype;
	};

	class VariableBuilder : public lib::IBuilder<Variable> {
	public:
		VariableBuilder& name(std::string name);
		VariableBuilder& address(lib::address address);
		VariableBuilder& datatype(const Datatype& datatype);

		Variable&& build() override;

	private:
		struct {
			bool name     : 1 = false;
			bool address  : 1 = false;
			bool datatype : 1 = false;
		} _status;
	};
}
