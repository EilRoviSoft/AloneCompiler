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

    public:
        ScopeContainer() {
            _layers.emplace_back();
        }

        ScopeContainer clone() const {
            ScopeContainer result;
            std::unordered_map<size_t, const IScopeElement*> elements_dict;

            for (const auto& it : _container) {
                auto ptr = it->clone();
                result._container.emplace_back();
            }

            return result;
        }

    private:
        std::list<ScopeElement> _container;
        std::vector<layer> _layers;
        std::unordered_map<size_t, const IScopeElement*> _search;
    };
}
