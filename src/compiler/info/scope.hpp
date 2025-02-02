#pragma once

//std
#include <list>
#include <string>
#include <unordered_map>

//compiler_info
#include "compiler/info/datatypes.hpp"
#include "compiler/info/layers.hpp"
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
	class Scope {
		friend class ScopeProxy;

	public:
		Scope() = default;
		Scope(const Scope&) = default;
		Scope(Scope&&) = default;
		~Scope();

	private:
		std::list<std::pair<layers_pos, ScopeElement>> _container;
		layers_container _layers;
		std::unordered_map<size_t, const IScopeElement*> _search;
	};
}
