#pragma once

//std
#include <queue>
#include <tuple>
#include <variant>

//compiler_info
#include "compiler/info/datatype_builder.hpp"
#include "compiler/info/function_builder.hpp"
#include "compiler/info/scope_container.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler::parser {
	enum QueryType : uint8_t {
		DatatypeDefinition,
		FunctionDefinition
	};
	using unit = std::variant<DatatypeBuilder, FunctionBuilder>;
	using queue = std::queue<std::tuple<QueryType, unit>>;
}

namespace amasm::compiler {
	class Parser {

	public:
		explicit Parser(Context& ctx);

		ScopeContainer parse(const token_vector& tokens) const;

	private:
		Context& _ctx;
	};
}
