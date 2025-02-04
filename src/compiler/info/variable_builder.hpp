#pragma once

//library
#include "library/types.hpp"
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/variable.hpp"

namespace amasm::compiler {
    class VariableBuilder : public lib::IBuilder<Variable> {
    public:
        VariableBuilder& name(std::string name);
        VariableBuilder& address(lib::address address);
        VariableBuilder& datatype(const Datatype& datatype);

        Variable&& get_product() override;

    private:
        struct {
            bool name     : 1 = false;
            bool address  : 1 = false;
            bool datatype : 1 = false;
        } _status;
    };
}
