#pragma once

//std
#include <vector>
#include <string>
#include <unordered_map>

//shared
#include "shared/types.hpp"

//compiler
#include "compiler/datatypes.hpp"

namespace amasm::compiler {
    struct variable {
        std::string name;
        shared::address address;
        const Datatype& type;
    };
    using variable_ptr = std::shared_ptr<variable>;

    variable_ptr make_variable(std::string name, const shared::address& address, const Datatype& type);

    class VariablesCollection {
    public:
        using local_scope = std::unordered_map<size_t, variable_ptr>;
        using local_scope_ptr = std::shared_ptr<local_scope>;

        void inherit_from(const VariablesCollection& another);
        void inherit_from(const std::initializer_list<variable_ptr>& list);

        void insert_local_variable(std::string name, const shared::address& address, const Datatype& type);
        void insert_global_variable(std::string name, const shared::address& address, const Datatype& type);

        const variable& get_variable(const std::string& key) const;

    private:
        std::vector<local_scope_ptr> _scopes;

        static void _insert_variable(const local_scope_ptr& where, variable_ptr&& var);
        static void _insert_variable(const local_scope_ptr& where, const variable_ptr& var);
    };
}
