#pragma once

//std
#include <string>

//alone
#include "data_types.hpp"
#include "general.hpp"

namespace alone::amasm {
	struct inst_info_t {
		size_t max_args_count, bit_depth;
	};

	struct argument_t {
		//direct -> name, immediate -> value, indirect_with_displacement -> name + value
		argument_type type;
		std::string name;
		ptrdiff_t value;
	};
	//using argument_t = std::variant<size_t, std::string, std::tuple<std::string, ptrdiff_t>>;

	data_type_ptr get_data_type(const std::string& str);
}
