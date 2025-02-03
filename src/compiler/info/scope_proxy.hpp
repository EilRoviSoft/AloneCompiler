#pragma once

//std
#include <list>
#include <ranges>

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/function.hpp"
#include "compiler/info/scope_element.hpp"
#include "compiler/info/variable.hpp"

namespace amasm::compiler {
    class ScopeProxy {
    public:
        ScopeProxy() = default;
        explicit ScopeProxy(ScopeContainer& parent);

        void set_parent(ScopeContainer& parent);

        template<scope_element_t T>
        void add(T&& elem, size_t local_id = 0) {
            auto derived = std::make_shared<T>(std::forward<T>(elem));
            auto base = std::static_pointer_cast<IScopeElement>(derived);
            add_from_ptr(std::move(base), local_id);
        }
        void add_from_ptr(ScopeElement&& elem, size_t local_id);

        const Datatype& get_datatype(const std::string& key, size_t local_id = 0) const;
        const Function& get_function(const std::string& key, size_t local_id = 0) const;
        const Variable& get_variable(const std::string& key, size_t local_id = 0) const;

        std::list<const Datatype*> get_all_datatypes() const;
        std::list<const Function*> get_all_functions() const;
        std::list<const Variable*> get_all_variables() const;

    private:
        ScopeContainer* _parent = nullptr;

        template<scope_element_t TRet>
        const TRet* _get_by_type(const std::string& key, size_t local_id) const;

        template<scope_element_t TRet, IScopeElement::Type TType>
        std::list<const TRet*> _get_all_by_type() const;
    };
}
