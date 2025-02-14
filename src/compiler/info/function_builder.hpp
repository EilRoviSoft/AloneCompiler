#pragma once

//library
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/function.hpp"
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_proxy.hpp"

namespace amasm::compiler {
    class LineVariantBuilder : public lib::IBuilder<LineVariant> {
    public:
        LineVariantBuilder& set_inst(InstDecl&& inst);
        LineVariantBuilder& set_label(std::string label);

        bool is_built() const override;

        LineVariant&& get_product() override;

    private:
        struct {
            bool value : 1 = false;
        } _is_set;
    };

    class FunctionBuilder : public lib::IBuilder<Function> {
    public:
        FunctionBuilder();

        FunctionBuilder& set_name(std::string name);
        FunctionBuilder& set_return_type(const Datatype& datatype);
        FunctionBuilder& add_argument_type(const Datatype& datatype);
        FunctionBuilder& set_scope(ScopeProxy proxy, size_t scope_id);
        // firstly specify scope
        FunctionBuilder& add_to_scope(ScopeElement&& elem);
        FunctionBuilder& add_line(LineVariant&& line);

        bool is_built() const override;

        Function&& get_product() override;

    private:
        ScopeProxy _proxy;

        struct {
            bool name        : 1 = false;
            bool return_type : 1 = false;
            bool scope       : 1 = false;
        } _is_set;
    };
}
