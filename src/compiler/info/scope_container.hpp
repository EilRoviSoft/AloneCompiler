#pragma once

//std
#include <cstdint>
#include <list>
#include <ranges>
#include <unordered_map>
#include <vector>

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/function.hpp"
#include "compiler/info/scope_element.hpp"
#include "compiler/info/scope_proxy.hpp"
#include "compiler/info/variable.hpp"

namespace amasm::compiler {
    class ScopeContainer {
        friend class ScopeProxy;
        friend class ConstScopeProxy;

    public:
        ScopeContainer() {
            _layers.emplace_back();
        }

        ScopeContainer clone() const {
            ScopeContainer clone;
            std::unordered_map<uintptr_t, const IScopeElement*> elements_dict;

            for (const auto& it : _container) {
                const auto& inserted = clone._container.emplace_back(it->clone());
                auto on_emplace = std::make_pair(reinterpret_cast<uintptr_t>(it.get()), inserted.get());
                elements_dict.emplace(on_emplace);
                clone._search.emplace(on_emplace);
            }

            clone._layers.resize(_layers.size());
            for (size_t i = 0; i < _layers.size(); i++)
                for (const auto& val : _layers[i] | std::views::values) {
                    auto address = reinterpret_cast<uintptr_t>(val);
                    const auto& elem = elements_dict.at(address);
                    auto hashed_key = val->hash();
                    clone._layers[i].emplace(hashed_key, elem);
                }

            // changing existing references to datatypes
            ScopeProxy proxy = clone;
            for (auto& it : clone._container) {
                if (it->type_id() == 1) {
                    auto datatype = std::dynamic_pointer_cast<Datatype>(it);
                    for (auto& pole : datatype->m_poles)
                        pole.m_datatype = &proxy.get_datatype(pole.datatype().name());
                } else if (it->type_id() == 2) {
                    auto function = std::dynamic_pointer_cast<Function>(it);
                    for (auto& type : function->m_types)
                        type = &proxy.get_datatype(type->name());
                } else if (it->type_id() == 3) {
                    auto variable = std::dynamic_pointer_cast<Variable>(it);
                    variable->m_datatype = &proxy.get_datatype(variable->datatype().name());
                }
            }

            return clone;
        }

    private:
        std::list<ScopeElement> _container;
        std::vector<layer> _layers;
        std::unordered_map<size_t, const IScopeElement*> _search;
    };
}
