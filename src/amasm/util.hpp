#pragma once

//std
#include <string>

//alone::amasm::info
#include "amasm/info.hpp"

namespace alone::amasm::util {
	inline bool is_alpha(char c) {
		return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
	}
	inline bool is_binary(char c) {
		return c == '0' || c == '1';
	}
	inline bool is_numeric(char c) {
		return c >= '0' && c <= '9';
	}
	inline bool is_hexadecimal(char c) {
		return is_numeric(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
	}
	inline bool is_alpha_numeric(char c) {
		return is_alpha(c) || is_numeric(c);
	}
	inline bool is_whitespace(char c) {
		return c == ' ' || c == '\n' || c == '\t';
	}

	inline std::string gen_str(char a, char b) {
		std::string result;

		result.resize(b - a + 1);
		for(char i = a; i <= b; ++i)
			result[i - a] = i;

		return result;
	}

	inline void sync_identifiers() {
		for(const auto& it : info::data_types)
			info::identifiers.try_emplace(it, info::data_type);
		for(const auto& it : info::instructions)
			info::identifiers.try_emplace(it, info::instruction);
	}
}
