#pragma once

//library
#include "library/types.hpp"
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/variable.hpp"

namespace amasm::compiler {
    class VariableBuilder : public lib::IBuilder<Variable> {
    public:
        VariableBuilder& set_name(std::string name);
        VariableBuilder& set_fixed_address(lib::address address);
        VariableBuilder& set_relative_address(std::string variable_name, lib::address offset);
        VariableBuilder& set_datatype(const Datatype& datatype);

        bool is_built() const override;

    private:
        struct {
            bool name     : 1 = false;
            bool address  : 1 = false;
            bool datatype : 1 = false;
        } _is_set;

        const char* get_error_message() const override;
    };
}
