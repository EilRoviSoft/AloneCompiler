#pragma once
//std
#include <set>
#include <map>
#include <string>

//alone::amasm::info
#include "info.hpp"

namespace alone::amasm::util {
	inline std::set<std::string> data_types = {
		"uint8", "uint16", "uint32", "uint64",
		"int8", "int16", "int32", "int64",
		"float32", "float64"
	};
	inline std::set<std::string> instructions;
	inline std::map<std::string, info::token_type> identifiers = {
		{ "sector", info::sector },
		{ "label", info::label },
		{ "func", info::function },
		{ "struct", info::container }
	};

	inline void add_data_type(const std::string& v) {
		data_types.emplace(v);
		identifiers.emplace(v, info::data_type);
	}
	inline void add_instruction(const std::string& v) {
		instructions.emplace(v);
		identifiers.emplace(v, info::instruction);
	}
}
