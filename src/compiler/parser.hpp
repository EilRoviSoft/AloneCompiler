#pragma once

//std
#include <queue>
#include <string>
#include <tuple>

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/function.hpp"
#include "compiler/info/scope_element.hpp"
#include "compiler/info/variable.hpp"

//compiler
#include "compiler/context.hpp"

namespace amasm::compiler {
	class Parser {
		using parse_queue = std::queue<std::tuple<std::string, const IScopeElement&>>;

	public:
		explicit Parser(Context& ctx);

		ScopeContainer parse(const token_vector& tokens) const;

	private:
		Context& _ctx;
	};
}
