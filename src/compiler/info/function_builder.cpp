#include "compiler/info/function_builder.hpp"

//std
#include <stdexcept>

namespace amasm::compiler {
    FunctionBuilder::FunctionBuilder() {
        m_product->m_types.emplace_back(nullptr);
    }

    FunctionBuilder& FunctionBuilder::set_name(std::string name) {
        m_product->m_name = std::move(name);
        _is_set.name = true;
        return *this;
    }
    FunctionBuilder& FunctionBuilder::set_return_type(const Datatype& datatype) {
        m_product->m_types.front() = &datatype;
        _is_set.return_type = true;
        return *this;
    }
    FunctionBuilder& FunctionBuilder::add_argument_type(const Datatype& datatype) {
        m_product->m_types.emplace_back(&datatype);
        return *this;
    }
    FunctionBuilder& FunctionBuilder::set_scope(ScopeProxy proxy, size_t scope_id) {
        _proxy = proxy;
        m_product->m_scope_id = scope_id;

        if (scope_id >= proxy.amount())
            proxy.resize_scopes(scope_id + 1);

        _is_set.scope = true;
        return *this;
    }
    FunctionBuilder& FunctionBuilder::add_to_scope(ScopeElement&& elem) {
        if (!_is_set.scope)
            throw std::runtime_error("you have to specify scope_id first");
        _proxy.add_from_ptr(std::move(elem), m_product->m_scope_id);

        return *this;
    }
    FunctionBuilder& FunctionBuilder::add_line(InstDecl&& decl) {
        m_product->m_lines.emplace_back(std::move(decl));
        return *this;
    }

    Function&& FunctionBuilder::get_product() {
        if (!_is_set.name || !_is_set.return_type || !_is_set.scope)
            throw std::runtime_error("you have to specify name, return_type and scope_id");

        return IBuilder::get_product();
    }
}
