#include "scope_proxy.hpp"

//std
#include <stdexcept>

//library
#include "library/general_functions.hpp"

//compiler_info
#include "compiler/info/scope_container.hpp"

namespace amasm::compiler {
    ScopeProxy::ScopeProxy(ScopeContainer& parent) :
        _parent(&parent) {
    }

    void ScopeProxy::set_parent(ScopeContainer& parent) { _parent = &parent; }

    void ScopeProxy::add_from_ptr(ScopeElement&& elem, size_t local_id) {
        const auto& inserted = _parent->_container.emplace_back(std::move(elem));
        auto on_emplace = std::make_pair(inserted->hash(), inserted.get());
        _parent->_search.emplace(on_emplace);
        _parent->_layers[local_id].emplace(on_emplace);
    }

    const IScopeElement& ScopeProxy::get_ptr(const std::string& key, size_t local_id) const {
        return *_get_ptr(key, local_id);
    }

    const Datatype& ScopeProxy::get_datatype(const std::string& key, size_t local_id) const {
        return *dynamic_cast<const Datatype*>(_get_ptr(key, local_id));
    }
    const Function& ScopeProxy::get_function(const std::string& key, size_t local_id) const {
        return *dynamic_cast<const Function*>(_get_ptr(key, local_id));
    }
    const Variable& ScopeProxy::get_variable(const std::string& key, size_t local_id) const {
        return *dynamic_cast<const Variable*>(_get_ptr(key, local_id));
    }

    std::list<const Datatype*> ScopeProxy::get_all_datatypes() const {
        return _get_all_by_type<Datatype, 1>();
    }
    std::list<const Function*> ScopeProxy::get_all_functions() const {
        return _get_all_by_type<Function, 2>();
    }
    std::list<const Variable*> ScopeProxy::get_all_variables() const {
        return _get_all_by_type<Variable, 3>();
    }

    size_t ScopeProxy::amount() const {
        return _parent->_layers.size();
    }
    void ScopeProxy::resize_scopes(size_t nsize) {
        _parent->_layers.resize(nsize);
    }

    const IScopeElement* ScopeProxy::_get_ptr(const std::string& key, size_t local_id) const {
        const IScopeElement* result;
        auto hashed_key = lib::hash_string(key);
        const auto& local = _parent->_layers[local_id];
        const auto& global = _parent->_layers.front();

        if (auto it = local.find(hashed_key); it != local.end())
            result = it->second;
        else if (auto jt = global.find(hashed_key); jt != global.end())
            result = jt->second;
        else
            throw std::runtime_error(key + " doesn't exist");

        return result;
    }
    template<scope_element_t TRet, size_t TypeId>
    std::list<const TRet*> ScopeProxy::_get_all_by_type() const {
        std::list<const TRet*> result;
        for (const auto& it : _parent->_container) {
            if (it->m_type_id == TypeId) {
                auto original = it.get();
                auto ptr = dynamic_cast<TRet*>(original);
                result.emplace_back(ptr);
            }
        }
        return result;
    }
}
