#pragma once

//compiler_info
#include "compiler/info/datatypes.hpp"
#include "compiler/info/scope_info.hpp"

namespace amasm::compiler {
	class Scope {
		friend class ScopeProxy;

	public:
		~Scope();

		const Datatype& get_datatype(const std::string& key) const;

	private:
		std::list<ScopeElement> _container;
		std::list<std::list<std::unordered_map<size_t, const IScopeElement*>>> _layers;
		std::unordered_map<size_t, const IScopeElement*> _search;
	};
}
