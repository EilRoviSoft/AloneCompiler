#pragma once

//std
#include <string>
#include <unordered_map>
#include <vector>

//compiler_info
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
    struct layers_pos {
        size_t idx, idy;
    };

    class Layer {
        friend class ScopeProxy;
    public:
        Layer(ScopeProxy& proxy);

        const IScopeElement* at(size_t hashed_key) const;

        void add_ptr(ScopeElement&& elem) const;

    private:
        ScopeProxy* _proxy;
        std::unordered_map<size_t, const IScopeElement*> _data;

        void _emplace(size_t hashed_key, const IScopeElement* elem);
    };

    struct layers_container {
        std::vector<std::vector<Layer>> data;
        size_t max_w = 0, max_h = 0;

        const IScopeElement* operator()(const layers_pos& pos, const std::string& key) const;
    };
}
