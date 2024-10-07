#pragma once

//std
#include <string>
#include <unordered_map>
#include <unordered_set>

//alone::amasm
#include "data_types.hpp"
#include "info.hpp"
#include "parse_rules.hpp"

namespace alone::amasm {
	//extern std::unordered_set<parse_flag_type> flags_with_size;
	extern std::unordered_set<char> singular_tokens;
	extern std::unordered_map<std::string, token_type> defined_tokens;
	extern std::unordered_map<std::string, parse_rule_ptr> rules;
	extern std::unordered_map<std::string, data_type_ptr> data_types;

	void add_data_type(const data_type_ptr& ptr);
	//void add_inst();

	void init_consts();
}
