#pragma once

//std
#include <tuple>

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/function.hpp"
#include "compiler/info/variable.hpp"
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
	class Parser {
	public:
		using parse_unit = const IScopeElement&;

	protected:

	};
}
