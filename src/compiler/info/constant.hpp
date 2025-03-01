#pragma once

//library
#include "library/types.hpp"

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
    class Constant : public IScopeElement {
        friend class ConstantBuilder;
        friend class ScopeContainer;

    public:
        Constant() : IScopeElement(4) {}

        const lib::byte_vector& data() const { return m_data; }

        ScopeElement clone() const override {
            return std::make_shared<Constant>(*this);
        }

    protected:
        lib::byte_vector m_data;
        const Datatype* m_datatype;
    };
}
