#pragma once

//std
#include <vector>

//library
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/datatypes.hpp"
#include "compiler/info/instructions.hpp"
#include "compiler/info/scope_info.hpp"

namespace amasm::compiler {
	class Function : public IScopeElement {
		friend class ScopeProxy;
		friend class FunctionBuilder;

	public:
		Function(const Function&) = default;
		Function(Function&&) = default;

		const Datatype& return_type();
		const Datatype& argument_type(size_t idx);
		const ScopeProxy& scope();
		const InstDecl& line(size_t idx);

	protected:
		// [0] for return type
		// [1..size] for arguments' types
		std::vector<const Datatype*> m_types;
		ScopeProxy m_scope;
		std::vector<InstDecl> m_lines;

	private:
		Function() = default;
	};

	class FunctionBuilder : public lib::IBuilder<Function> {
	public:
		FunctionBuilder();

		FunctionBuilder& name(std::string name);
		FunctionBuilder& return_type(const Datatype& datatype);
		FunctionBuilder& add_argument_type(const Datatype& datatype);
		FunctionBuilder& scope(Scope& scope);
		// firstly specify scope
		FunctionBuilder& add_to_scope(ScopeElement&& elem);
		FunctionBuilder& add_line(InstDecl&& decl);

		Function&& build() override;

	private:
		struct {
			bool name        : 1 = false;
			bool return_type : 1 = false;
			bool scope       : 1 = false;
		} _status;
	};
}
