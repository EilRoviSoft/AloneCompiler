#include "variables.hpp"

//std
#include <stdexcept>

//shared
#include "shared/general_functions.hpp"

namespace amasm::compiler {
    variable_ptr make_variable(std::string name, datatype_ptr type, const shared::Address& address) {
        return std::make_shared<variable>(std::move(name), std::move(type), address);
    }

    void VariablesCollection::inherit_from(const VariablesCollection& another) {
        _scopes.clear();
        _scopes.reserve(another._scopes.size() + 1);
        _scopes.append_range(another._scopes);
        if (!another._scopes.back()->empty())
            _scopes.emplace_back(std::make_shared<local_scope>());
    }
    void VariablesCollection::inherit_from(const std::initializer_list<variable_ptr>& list) {
        _scopes.clear();
        _scopes.reserve(2);

        _scopes.emplace_back(std::make_shared<local_scope>());
        for (const auto& ptr : list)
            _insert_variable(_scopes.back(), ptr);

        // to add new empty scope with variables
        _scopes.emplace_back(std::make_shared<local_scope>());
    }

    void VariablesCollection::insert_local_variable(std::string name,
                                                    datatype_ptr type,
                                                    const shared::Address& address) {
        _insert_variable(_scopes.back(), make_variable(std::move(name), std::move(type), address));
    }
    void VariablesCollection::insert_global_variable(std::string name,
                                                     datatype_ptr type,
                                                     const shared::Address& address) {
        _insert_variable(_scopes.front(), make_variable(std::move(name), std::move(type), address));
    }

    void VariablesCollection::_insert_variable(const local_scope_ptr& where, variable_ptr&& var) {
        const size_t temp = shared::hash_string(var->name);
        where->emplace(temp, std::move(var));
    }
    void VariablesCollection::_insert_variable(const local_scope_ptr& where, const variable_ptr& var) {
        const size_t temp = shared::hash_string(var->name);
        where->emplace(temp, var);
    }

    variable_ptr VariablesCollection::get_variable(const std::string& key) const {
        variable_ptr result = nullptr;
        const size_t hashed_key = shared::hash_string(key);

        for (const auto& scope : _scopes)
            if (const auto it = scope->find(hashed_key); it != scope->end()) {
                result = it->second;
                break;
            }

        return result;
    }
}
