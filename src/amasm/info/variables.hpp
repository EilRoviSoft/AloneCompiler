#pragma once

//std
#include <vector>
#include <string>
#include <unordered_map>

//shared
#include "shared/types.hpp"

//amasm
#include "amasm/info/datatypes.hpp"

namespace alone::amasm {
    struct variable_t {
        std::string name;
        datatype_ptr type;
        shared::address_t address;
    };
    using variable_ptr = std::shared_ptr<variable_t>;

    variable_ptr make_variable(std::string name, datatype_ptr type, const shared::address_t& address);

    class Variables {
    public:
        using local_scope_t = std::unordered_map<size_t, variable_ptr>;
        using local_scope_ptr = std::shared_ptr<local_scope_t>;

        void inherit_from(const Variables& another);
        void inherit_from(const std::initializer_list<variable_ptr>& list);

        void insert_local_variable(std::string name, datatype_ptr type, const shared::address_t& address);
        void insert_global_variable(std::string name, datatype_ptr type, const shared::address_t& address);

        variable_ptr get_variable(const std::string& key) const;

    private:
        std::vector<local_scope_ptr> _scopes;

        static void _insert_variable(const local_scope_ptr& where, variable_ptr&& var);
        static void _insert_variable(const local_scope_ptr& where, const variable_ptr& var);
    };
}
