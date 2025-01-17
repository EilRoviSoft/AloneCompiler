#pragma once

//std
#include <vector>
#include <string>
#include <unordered_map>

//shared
#include "shared/types.hpp"

//compiler
#include "compiler/info/datatypes.hpp"

namespace amasm::compiler {
    struct variable {
        std::string name;
        datatype_ptr type;
        shared::address address;
    };
    using variable_ptr = std::shared_ptr<variable>;

    variable_ptr make_variable(std::string name, datatype_ptr type, const shared::address& address);

    class VariablesCollection {
    public:
        using local_scope = std::unordered_map<size_t, variable_ptr>;
        using local_scope_ptr = std::shared_ptr<local_scope>;

        void inherit_from(const VariablesCollection& another);
        void inherit_from(const std::initializer_list<variable_ptr>& list);

        void insert_local_variable(std::string name, datatype_ptr type, const shared::address& address);
        void insert_global_variable(std::string name, datatype_ptr type, const shared::address& address);

        variable_ptr get_variable(const std::string& key) const;

    private:
        std::vector<local_scope_ptr> _scopes;

        static void _insert_variable(const local_scope_ptr& where, variable_ptr&& var);
        static void _insert_variable(const local_scope_ptr& where, const variable_ptr& var);
    };
}
