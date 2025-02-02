#pragma once

//std
#include <list>
#include <ranges>

//compiler_info
#include "compiler/info/datatypes.hpp"
#include "compiler/info/functions.hpp"
#include "compiler/info/layers.hpp"
#include "compiler/info/scope_element.hpp"
#include "compiler/info/variables.hpp"

namespace amasm::compiler {
    class ScopeProxy {
    public:
        ScopeProxy() = default;
        explicit ScopeProxy(Scope& parent);

        void set_parent(Scope& parent);

        // expanding in height
        void alloc_new_layer();
        // expanding in width
        void alloc_new_scope();

        void set_global_scope();

        const std::unordered_map<size_t, const IScopeElement*>& get_layer();

        template<scope_element_t T>
        void add_element(T&& elem) {
            auto ptr = std::make_shared<IScopeElement>(std::forward<T>(elem));
            add_ptr(std::move(ptr));
        }
        void add_ptr(ScopeElement&& elem);

        std::list<const Datatype*> get_datatypes() const;
        std::list<const Function*> get_functions() const;
        std::list<const Variable*> get_variables() const;

    private:
        Scope* _parent = nullptr;
        layers_pos _pos = { .idx = 0, .idy = 0 };

        template<scope_element_t T>
        std::list<const T*> _get_by_type(IScopeElement::Type type) const;
    };
}
