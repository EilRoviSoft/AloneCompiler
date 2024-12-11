#include "instructions.hpp"

//alone::isa
#include "isa/type_related.hpp"

namespace alone::lib {
	std::unordered_map<std::string, inst_ptr> instructions_by_name;
	std::unordered_map<inst_code_t, inst_ptr> instructions_by_code;

	void init_instruction(std::string name, inst_code_t code, size_t max_args_count, size_t bit_depth, inst_func_t pred) {
		auto ptr = std::make_shared<inst_t>(std::move(name), code, max_args_count, bit_depth, std::move(pred));
		instructions_by_name.emplace(ptr->name, ptr);
		instructions_by_code.emplace(ptr->code, ptr);
	}
	void init_instruction(inst_t&& inst) {
		auto ptr = std::make_shared<inst_t>(std::move(inst));
		instructions_by_name.emplace(ptr->name, ptr);
		instructions_by_code.emplace(ptr->code, ptr);
	}

	void init_isa() {
		for (auto& it : isa::generate_type_related_isa())
			init_instruction(std::move(it));
	}
}
