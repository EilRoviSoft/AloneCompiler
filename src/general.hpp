#pragma once

//std
#include <functional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

//alone
#include "error_codes.hpp"

namespace alone {
	//enums

	enum class literal_type {
		none = 0,
		word,
		binary, decimal, hexadecimal, floating
	};

	//alias types

	using byte_array_t = std::vector<std::byte>;
	//using string_array_t = std::vector<std::string>;

	//objects

	inline auto string_hasher = std::hash<std::string>();

	//functions

	constexpr size_t gen_mask(size_t pos) {
		return 1ull << pos;
	}
	constexpr size_t gen_mask(std::initializer_list<size_t> positions) {
		size_t result = 0;
		for (auto it : positions)
			result += 1ull << it;
		return result;
	}
	constexpr size_t gen_mask(size_t a, size_t b) {
		size_t result = 0;
		for (size_t i = a; i <= b; ++i)
			result += 1ull << i;
		return result;
	}

	constexpr bool is_alpha(char c) {
		return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
	}
	constexpr bool is_binary(char c) {
		return c == '0' || c == '1';
	}
	constexpr bool is_numeric(char c) {
		return c >= '0' && c <= '9';
	}
	constexpr bool is_hexadecimal(char c) {
		return is_numeric(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
	}
	constexpr bool is_alpha_numeric(char c) {
		return is_alpha(c) || is_numeric(c);
	}
	constexpr bool is_whitespace(char c) {
		return c == ' ' || c == '\n' || c == '\t';
	}

	inline literal_type check_type(const std::string& l) {
		literal_type result;

		if (is_alpha(l.front())) {
			for (auto c : l | std::views::drop(1)) {
				if (!is_alpha_numeric(c))
					throw GENERAL_WRONG_WORD_VALUE;
			}
			result = literal_type::word;
		} else if (is_numeric(l.front())) {
			if (l.contains('.')) {
				size_t i;
				for (i = 1; l[i] != '.'; ++i) {
					if (!is_numeric(l[i]))
						throw GENERAL_WRONG_FLOATING_VALUE;
				}
				for (++i; i != l.size(); ++i) {
					if (!is_numeric(l[i]))
						throw GENERAL_WRONG_FLOATING_VALUE_AFTER_DOT;
				}
				result = literal_type::floating;
			} else if (l.starts_with("0b")) {
				for (auto c : l | std::views::drop(2)) {
					if (c != '0' && c != '1')
						throw GENERAL_WRONG_BINARY_VALUE;
				}
				result = literal_type::binary;
			} else if (l.starts_with("0x")) {
				for (auto c : l | std::views::drop(2)) {
					if (!is_hexadecimal(c))
						throw GENERAL_WRONG_HEX_VALUE;
				}
				result = literal_type::hexadecimal;
			} else {
				for (auto c : l) {
					if (!is_numeric(c))
						throw GENERAL_WRONG_DECIMAL_VALUE;
				}
				result = literal_type::decimal;
			}
		} else
			result = literal_type::none;

		return result;
	}

	inline std::string gen_str(char a, char b) {
		std::string result;
		result.resize(b - a + 1);
		for (char i = a; i <= b; ++i)
			result[i - a] = i;
		return result;
	}

	//structs

	template<typename... Ts> struct overloaded : Ts... {
		using Ts::operator()...;
	};

	template<typename T> struct array_t {
		size_t size;
		T* ptr;
	};

	template<typename T> struct limit_t {
		T n, max;

		bool is_lim() {
			return n == max;
		}
	};
}
