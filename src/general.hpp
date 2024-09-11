#pragma once

//std
#include <functional>
#include <string>
#include <unordered_set>

namespace alone {
	template<typename... Ts>
	struct overloaded : Ts... { using Ts::operator()...; };

	inline auto string_hasher = std::hash<std::string>();

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

	/*inline std::unordered_set<char> gen_set(char a, char b) {
		std::unordered_set<char> result;
		for (char i = a; i <= b; ++i)
			result.emplace(i);
		return result;
	}*/

	template <typename T>
	struct array_t {
		size_t size;
		T* ptr;
	};

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

	constexpr bool check_on_property(const std::string& s, const std::function<bool(char)>& property) {
		bool result = true;
		for (auto c : s)
			if (!property(c)) {
				result = false;
				break;
			}
		return result;
	}

	inline std::string gen_str(char a, char b) {
		std::string result;

		result.resize(b - a + 1);
		for (char i = a; i <= b; ++i)
			result[i - a] = i;

		return result;
	}
}