#pragma once

//std
#include <concepts>
#include <list>
#include <string>

//library
#include "library/builder.hpp"
#include "library/general_functions.hpp"

namespace amasm::compiler {
    class IScopeElement {
        friend class Scope;

    public:
        virtual ~IScopeElement() = default;

        const std::string& name() const;

        virtual size_t hash() const;

    protected:
        std::string m_name;
    };
    using ScopeElement = std::shared_ptr<IScopeElement>;

    template<typename T>
    concept scope_element_t = requires {
        std::derived_from<T, IScopeElement>;
    };

    class ScopeProxy {
    public:
        ScopeProxy() = default;
        // calls set_parent instead of you
        explicit ScopeProxy(Scope& parent);

        void set_parent(Scope& parent);

        void alloc_new_layer();
        void alloc_new_scope();

        void set_global_scope();
        void set_local_scope();

        template<scope_element_t T>
        void add_element(T&& elem);
        void add_ptr(ScopeElement&& ptr);

    private:
        Scope* _parent = nullptr;
        std::list<std::unordered_map<size_t, const IScopeElement*>>::iterator _layer;
    };
}
