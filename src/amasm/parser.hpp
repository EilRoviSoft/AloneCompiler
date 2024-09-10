#pragma once

//std
#include <any>
#include <cstddef>
#include <functional>
#include <span>
#include <vector>

//alone::amasm::info
#include "amasm/info.hpp"

namespace alone::amasm {
	using token_array_t = std::vector<info::token_t>;
	using inst_dispatcher_t = std::function<std::any(const token_array_t& tokens, size_t& i)>;

	//TODO: token matching
	class Parser {
	public:
		static std::vector<std::byte> parse(const token_array_t& tokens);

	private:
		static std::unordered_set<std::string, info::var_t> _variables;
		static std::unordered_map<std::string, inst_dispatcher_t> _generator;

		//[%var +/- offset]
		static info::address_offset_t _read_address_offset(const token_array_t& tokens, size_t& i);
	};
}
