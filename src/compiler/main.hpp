#pragma once
//std
#include <vector>
#include <string>

//alone::compiler::info
#include "compiler/info.hpp"

namespace alone::compiler {
	struct token_t {
		std::string v;
		info::token_type t;
	};
	using token_array = std::vector<token_t>;
}
