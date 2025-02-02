#pragma once

//std
#include <list>
#include <string>
#include <unordered_map>

//library
#include "library/general_functions.hpp"

//compiler_info
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
	class Datatype : public IScopeElement {
		friend class DatatypeBuilder;

		struct pole_info {
			std::string name;
			const Datatype& type;
			ptrdiff_t offset;
		};

	public:
		Datatype();

		size_t size() const { return m_size; }
		const pole_info& pole(const std::string& pole_name) const {
			size_t hash = lib::hash_string(pole_name);
			return m_poles_search.at(hash);
		}

	protected:
		size_t m_size;
		std::list<pole_info> m_poles;
		std::unordered_map<size_t, const pole_info&> m_poles_search;
	};
}
