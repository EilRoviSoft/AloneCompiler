#pragma once

//std
#include <fstream>
#include <string>
#include <tuple>

//alone::amasm
#include "info.hpp"

namespace alone::amasm {
	class Parser {
	public:
		static bool init();

	private:
		static void _init_basic_rules();

	public:
		//TODO: make stack-based flags dispatching
		static bool _match_rules(const std::vector<std::string>& what, const std::vector<token_t>& tokens, size_t start_idx);
	};
}
