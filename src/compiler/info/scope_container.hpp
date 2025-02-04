#pragma once

//std
#include <cstdint>
#include <list>
#include <ranges>
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
            std::unordered_map<uintptr_t, const IScopeElement*> elements_dict;

            for (const auto& it : _container) {
                const auto& inserted = result._container.emplace_back(it->clone());
                auto on_emplace = std::make_pair(reinterpret_cast<uintptr_t>(it.get()), inserted.get());
                elements_dict.emplace(on_emplace);
                result._search.emplace(on_emplace);
            }

            result._layers.resize(_layers.size());
            for (size_t i = 0; i < _layers.size(); i++)
                for (const auto& val : _layers[i] | std::views::values) {
                    auto address = reinterpret_cast<uintptr_t>(val);
                    const auto& elem = elements_dict.at(address);
                    auto hashed_key = val->hash();
                    result._layers[i].emplace(hashed_key, elem);
                }

            return result;
        }

    private:
        std::list<ScopeElement> _container;
        std::vector<layer> _layers;
        std::unordered_map<size_t, const IScopeElement*> _search;
    };
}
