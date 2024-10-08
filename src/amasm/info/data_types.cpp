#include "data_types.hpp"

//alone
#include "error_codes.hpp"

namespace alone::amasm {
	pole_t::pole_t(std::string n, data_type_ptr t, size_t o) :
		name(std::move(n)),
		type(std::move(t)),
		offset(o) {}

	data_type_t::data_type_t(std::string n, size_t s) :
		name(std::move(n)),
		size(s) {}
	data_type_t::data_type_t(std::string n, std::vector<pole_t> v) :
		name(std::move(n)),
		poles(std::move(v)) {
		if (!poles.empty()) {
			size = 0;
			for (const pole_t& it : poles)
				size += it.type->size;
			for (size_t i = 0; i != poles.size() - 1; ++i)
				poles.back().offset + poles.back().type->size;
		} else
			throw AMASM_DATA_TYPES_WRONG_CONSTRUCTOR_USAGE;
	}

	void data_type_t::add_pole(const std::string& n, const data_type_ptr& t, size_t o) {
		poles.emplace_back(n, t, poles.empty() ? o : poles.back().offset + poles.back().type->size);
	}
}
