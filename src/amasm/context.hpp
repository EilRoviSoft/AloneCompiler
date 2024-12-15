#pragma once

//std
#include <unordered_map>
#include <string>

//amasm
#include "library/tokens.hpp"

namespace alone::amasm {
	class AmasmContext {
	public:
		std::unordered_map<std::string, token_type> defined_tokens;

		void init();

	private:
		void _init_defined_tokens();
	};
}
