#include "data_types.hpp"

//std
#include <stdexcept>

//alone
#include "general.hpp"

namespace alone::amasm {
	pole_t::pole_t(std::string n, data_type_ptr t, size_t o) :
		name(std::move(n)),
		type(std::move(t)),
		offset(o) {
	}

	data_type_t::data_type_t(std::string n, size_t s) :
		name(std::move(n)),
		size(s) {
		if (s == 0) throw std::runtime_error("info.cpp: struct size should not be 0.");
	}
	data_type_t::data_type_t(std::string n, std::vector<pole_t> v) :
		name(std::move(n)),
		poles(std::move(v)) {
		if (!poles.empty()) {
			size = 0;
			for (const auto& it : poles) size += it.type->size;
		} else throw std::runtime_error("info.cpp: poles should not be empty, use another constructor instead");
	}

	size_t data_type_hasher::operator()(const data_type_ptr& val) const {
		return string_hasher(val->name);
	}
}
