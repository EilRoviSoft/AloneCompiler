#pragma once

//std
#include <variant>
#include <vector>

//compiler_info
#include "compiler/info/datatype.hpp"
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
    class LineVariant {
        friend class LineVariantBuilder;

    public:
        enum Type : uint8_t {
            None,
            Inst,
            Label
        };

        const InstDecl& inst() const { return std::get<InstDecl>(m_value); }
        const std::string& label() const { return std::get<std::string>(m_value); }

        Type type() const { return m_type; }

    protected:
        std::variant<InstDecl, std::string> m_value;
        Type m_type = None;
    };

    class Function : public IScopeElement {
        friend class FunctionBuilder;
        friend class ScopeContainer;

    public:
        Function() : IScopeElement(2) {}

        std::string fullname() const {
            std::string result;
            size_t args_count = m_types.size();
            result += '@' + m_name + '(';
            if (args_count > 1) {
                for (size_t i = 1; i < args_count - 1; i++)
                    result += m_types[i]->name() + ',';
                result += m_types.back()->name();
            }
            result += ')';
            return result;
        }

        const Datatype& return_type() const { return *m_types.front(); }
        const Datatype& argument_type(size_t idx) const { return *m_types[idx + 1]; }
        size_t arguments_count() const { return m_types.size() - 1; }
        size_t arguments_size() const {
            size_t result = 0;
            for (size_t i = 1; i < m_types.size(); i++)
                result += m_types[i]->size();
            return result;
        }

        size_t scope_id() const { return m_scope_id; }

        const LineVariant& line(size_t idx) const { return m_lines[idx]; }
        size_t lines_size() const { return m_lines.size(); }

        ScopeElement clone() const override {
            return std::make_shared<Function>(*this);
        }

    protected:
        // [0] for return type
        // [1 .. size - 1] for arguments' types
        std::vector<const Datatype*> m_types;
        size_t m_scope_id = 0;
        std::vector<LineVariant> m_lines;
    };
}
