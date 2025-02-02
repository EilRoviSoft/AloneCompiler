#include "scope_info.hpp"

//compiler_info
#include "compiler/info/scope.hpp"

namespace amasm::compiler {
    //IScopeElement

    const std::string& IScopeElement::name() const { return m_name; }

    size_t IScopeElement::hash() const { return lib::hash_string(m_name); }

    // ScopeProxy

    ScopeProxy::ScopeProxy(Scope& parent) {
        set_parent(parent);
    }

    void ScopeProxy::set_parent(Scope& parent) { _parent = &parent; }

    void ScopeProxy::alloc_new_layer() {
        _parent->_layers.emplace_back();
    }
    void ScopeProxy::alloc_new_scope() {
        _parent->_layers.back().emplace_back();
    }

    void ScopeProxy::set_global_scope() {
        _layer = _parent->_layers.front().begin();
    }
    void ScopeProxy::set_local_scope() {
        _layer = _parent->_layers.back().end();
    }

    template<scope_element_t T>
    void ScopeProxy::add_element(T&& elem) {
        const auto ptr = std::make_shared<IScopeElement>(std::forward<T>(elem));
        add_ptr(std::move(ptr));
    }
    void ScopeProxy::add_ptr(ScopeElement&& ptr) {
        const IScopeElement& val = *_parent->_container.emplace_back(std::move(ptr));
        const auto pair = std::make_pair(val.hash(), &val);
        _layer->emplace(pair);
        _parent->_search.emplace(pair);
    }
}
