#pragma once

//std
#include <string>
#include <unordered_map>
#include <unordered_set>

//alone::amasm
#include "data_types.hpp"
#include "enums.hpp"

namespace alone::amasm {
	//extern std::unordered_set<parse_flag_type> flags_with_size;
	extern std::unordered_set<char> singular_tokens;
	extern std::unordered_set<std::string> instructions;
	extern std::unordered_map<std::string, token_type> defined_tokens;
	extern std::unordered_map<std::string, data_type_ptr> data_types;
	extern std::unordered_map<std::string, std::vector<token_type>> rules;

	void add_data_type(const data_type_ptr& ptr);

	void init_consts();
}
