#include "function_builder.hpp"

//std
#include <stdexcept>

namespace amasm::compiler {
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
    FunctionBuilder& FunctionBuilder::scope(ScopeProxy proxy, size_t scope_id) {
        _proxy = proxy;
        m_product->m_scope_id = scope_id;
        _status.scope = true;
        return *this;
    }
    FunctionBuilder& FunctionBuilder::add_to_scope(ScopeElement&& elem) {
        if (!_status.scope)
            throw std::runtime_error("you have to specify scope_id first");
        _proxy.add_from_ptr(std::move(elem), m_product->m_scope_id);

        return *this;
    }
    FunctionBuilder& FunctionBuilder::add_line(InstDecl&& decl) {
        m_product->m_lines.emplace_back(std::move(decl));
        return *this;
    }

    Function&& FunctionBuilder::get_product() {
        if (!_status.name || !_status.return_type || !_status.scope)
            throw std::runtime_error("you have to specify name, return_type and scope_id");

        return IBuilder::get_product();
    }
}
