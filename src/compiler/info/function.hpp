#pragma once

//std
#include <vector>

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
    class Function : public IScopeElement {
        friend class FunctionBuilder;

    public:
        Function() : IScopeElement(2) {}

        const Datatype& return_type() { return *m_types.front(); }
        const Datatype& argument_type(size_t idx) { return *m_types[idx + 1]; }
        size_t scope_id() { return m_scope_id; }
        const InstDecl& line(size_t idx) { return m_lines[idx]; }

        ScopeElement clone() const override {
            return std::make_shared<IScopeElement>(*this);
        }

    protected:
        // [0] for return type
        // [1..size] for arguments' types
        std::vector<const Datatype*> m_types;
        size_t m_scope_id = 0;
        std::vector<InstDecl> m_lines;
    };
}
