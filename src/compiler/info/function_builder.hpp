#pragma once

//library
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/function.hpp"
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_proxy.hpp"

namespace amasm::compiler {
    class FunctionBuilder : public lib::IBuilder<Function> {
    public:
        FunctionBuilder();

        FunctionBuilder& name(std::string name);
        FunctionBuilder& return_type(const Datatype& datatype);
        FunctionBuilder& add_argument_type(const Datatype& datatype);
        FunctionBuilder& scope(ScopeProxy proxy, size_t scope_id);
        // firstly specify scope
        FunctionBuilder& add_to_scope(ScopeElement&& elem);
        FunctionBuilder& add_line(InstDecl&& decl);

        Function&& get_product() override;

    private:
        ScopeProxy _proxy;

        struct {
            bool name        : 1 = false;
            bool return_type : 1 = false;
            bool scope       : 1 = false;
        } _status;
    };
}
