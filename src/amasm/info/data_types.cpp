#include "data_types.hpp"

//std
#include <stdexcept>

//alone
#include "error_codes.hpp"

namespace alone::amasm {
	pole_t::pole_t(std::string n, data_type_ptr t, size_t o) :
		name(std::move(n)),
		type(std::move(t)),
		offset(o) {
	}

	data_type_t::data_type_t(std::string n, size_t s) :
		name(std::move(n)),
		size(s) {
		if (s == 0)
			throw AMASM_DATA_TYPES_EMPTY_STRUCT;
	}
	data_type_t::data_type_t(std::string n, std::vector<pole_t> v) :
		name(std::move(n)),
		poles(std::move(v)) {
		if (!poles.empty()) {
			size = 0;
			for (const pole_t& it : poles) size += it.type->size;
		} else throw AMASM_DATA_TYPES_WRONG_CONSTRUCTOR_USAGE;
	}
}
