#include "variables.hpp"

//std
#include <stdexcept>

//shared
#include "shared/general_functions.hpp"

namespace alone::amasm {
    variable_ptr make_variable(std::string name, datatype_ptr type, const shared::address_t& address) {
        return std::make_shared<variable_t>(std::move(name), std::move(type), address);
    }

    void Variables::inherit_from(const Variables& another) {
        _scopes.reserve(another._scopes.size() + 1);
        _scopes.append_range(another._scopes);
        _scopes.emplace_back();
    }

    void Variables::insert_local_variable(std::string name, datatype_ptr type, const shared::address_t& address) {
        _insert_variable(_scopes.back(), make_variable(std::move(name), std::move(type), address));
    }
    void Variables::insert_global_variable(std::string name, datatype_ptr type, const shared::address_t& address) {
        _insert_variable(_scopes.front(), make_variable(std::move(name), std::move(type), address));
    }
    void Variables::_insert_variable(const local_scope_ptr& where, variable_ptr&& var) {
        const size_t temp = shared::hash_string(var->name);
        where->emplace(temp, std::move(var));
    }

    variable_ptr Variables::get_variable(const std::string& key) const {
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
