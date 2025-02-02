#include "scope_proxy.hpp"

//compiler_info
#include "compiler/info/scope.hpp"

namespace amasm::compiler {
    // ScopeProxy

    ScopeProxy::ScopeProxy(Scope& parent) :
        _parent(&parent) {
    }

    void ScopeProxy::set_parent(Scope& parent) { _parent = &parent; }

    void ScopeProxy::alloc_new_layer() {
        _parent->_layers.data[_pos.idy].emplace_back(*this);
        _pos.idx++;
    }
    void ScopeProxy::alloc_new_scope() {
        _parent->_layers.data.emplace_back();
        _pos.idy++;
    }

    void ScopeProxy::set_global_scope() {
        _pos = { .idx = 0, .idy = 0 };
    }

    void ScopeProxy::add_ptr(ScopeElement&& elem) {
        const auto& inserted = _parent->_container.emplace_back(_pos, std::move(elem));
        _parent->_layers.data[_pos.idy][_pos.idx]._emplace(inserted.second->hash(), inserted.second.get());
        _parent->_search.emplace(inserted.second->hash(), inserted.second.get());
    }

    std::list<const Datatype*> ScopeProxy::get_datatypes() const {
        return _get_by_type<Datatype>(IScopeElement::Type::Datatype);
    }
    std::list<const Function*> ScopeProxy::get_functions() const {
        return _get_by_type<Function>(IScopeElement::Type::Function);
    }
    std::list<const Variable*> ScopeProxy::get_variables() const {
        return _get_by_type<Variable>(IScopeElement::Type::Variable);
    }

    template<scope_element_t T>
    std::list<const T*> ScopeProxy::_get_by_type(IScopeElement::Type type) const {
        std::list<const T*> result;
        for (const auto& val : _parent->_container | std::views::values)
            if (val->_type == type) {
                auto ptr = dynamic_cast<const T*>(val.get());
                result.emplace_back(ptr);
            }
        return result;
    }
}
