#pragma once

//std
#include <functional>
#include <unordered_map>
#include <memory>

//alone
#include "general.hpp"
#include "vm_context.hpp"

namespace alone {
	using inst_func_t = std::function<size_t(const context_t&, const std::array<argument_type, 4>&)>;

	struct inst_t {
		std::string name;
		inst_code_t code;
		size_t max_args_count;
		size_t bit_depth;
		inst_func_t pred;
	};

	using inst_ptr = std::shared_ptr<inst_t>;

	extern std::unordered_map<std::string, inst_ptr> instructions_by_name;
	extern std::unordered_map<inst_code_t, inst_ptr> instructions_by_code;
}
