#pragma once

//std
#include <list>
#include <unordered_map>
#include <vector>

//compiler_info
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
    class ScopeContainer {
        friend class ScopeProxy;

    private:
        std::list<ScopeElement> _container;
        std::vector<layer> _layers;
        std::unordered_map<size_t, const IScopeElement&> _search;
    };
}
