#pragma once

//std
#include <functional>
#include <unordered_map>
#include <memory>

//alone
#include "general.hpp"
#include "vm_context.hpp"

namespace alone {
	using inst_func_t = std::function<size_t(const context_t&)>;

	struct inst_t {
		const std::string name;
		const inst_code_t code;
		const size_t max_args_count, bit_depth;
		const inst_func_t func;
	};
	using inst_ptr = std::shared_ptr<inst_t>;

	extern std::unordered_map<std::string, inst_ptr> instructions_by_name;
	extern std::unordered_map<inst_code_t, inst_ptr> instructions_by_code;

	void init_instruction(std::string name,
						  inst_code_t code,
						  size_t max_args_count,
						  size_t bit_depth,
						  inst_func_t func);
}
