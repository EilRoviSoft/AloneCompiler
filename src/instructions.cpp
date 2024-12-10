#include "instructions.hpp"

namespace alone {
	std::unordered_map<std::string, inst_ptr> instructions_by_name;
	std::unordered_map<inst_code_t, inst_ptr> instructions_by_code;
}
