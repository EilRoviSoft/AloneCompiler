#pragma once

//std
#include <queue>
#include <string>
#include <tuple>
#include <variant>

//compiler_info
#include "compiler/info/datatype_builder.hpp"
#include "compiler/info/function_builder.hpp"
#include "compiler/info/scope_container.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler {
	using parse_unit = std::variant<DatatypeBuilder, FunctionBuilder>;
	using parse_queue = std::queue<std::tuple<std::string, parse_unit>>;

	class Parser {

	public:
		explicit Parser(Context& ctx);

		ScopeContainer parse(const token_vector& tokens) const;

	private:
		Context& _ctx;
	};
}
