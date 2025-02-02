#include "functions.hpp"

//std
#include <stdexcept>

namespace amasm::compiler {
    // Function

    const Datatype& Function::return_type() { return *m_types.front(); }
    const Datatype& Function::argument_type(size_t idx) { return *m_types[idx + 1]; }
    const ScopeProxy& Function::scope() { return m_scope; }
    const InstDecl& Function::line(size_t idx) { return m_lines[idx]; }

    // FunctionBuilder

    FunctionBuilder::FunctionBuilder() {
        m_product->m_types.emplace_back(nullptr);
    }

    FunctionBuilder& FunctionBuilder::name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    FunctionBuilder& FunctionBuilder::return_type(const Datatype& datatype) {
        m_product->m_types.front() = &datatype;
        _status.return_type = true;
        return *this;
    }
    FunctionBuilder& FunctionBuilder::add_argument_type(const Datatype& datatype) {
        m_product->m_types.emplace_back(&datatype);
        return *this;
    }
    FunctionBuilder& FunctionBuilder::scope(Scope& scope) {
        m_product->m_scope.set_parent(scope);
        _status.scope = true;
        return *this;
    }
    FunctionBuilder& FunctionBuilder::add_to_scope(ScopeElement&& elem) {
        if (!_status.scope)
            throw std::runtime_error("you have to specify scope first");
        m_product->m_scope.add_ptr(std::move(elem));
        return *this;
    }
    FunctionBuilder& FunctionBuilder::add_line(InstDecl&& decl) {
        m_product->m_lines.emplace_back(std::move(decl));
        return *this;
    }

    Function&& FunctionBuilder::build() {
        if (!_status.name || !_status.return_type || !_status.scope)
            throw std::runtime_error("you have to specify name, return_type and scope");

        return IBuilder::build();
    }
}
