#include "compiler/info/variable_builder.hpp"

//std
#include <stdexcept>

namespace amasm::compiler {
    VariableBuilder& VariableBuilder::set_name(std::string name) {
        m_product->m_name = std::move(name);
        _is_set.name = true;
        return *this;
    }
    VariableBuilder& VariableBuilder::set_address(lib::address address) {
        m_product->m_address = address;
        _is_set.address = true;
        return *this;
    }
    VariableBuilder& VariableBuilder::set_datatype(const Datatype& datatype) {
        m_product->m_datatype = &datatype;
        _is_set.datatype = true;
        return *this;
    }

    Variable&& VariableBuilder::get_product() {
        if (!_is_set.name || !_is_set.address || !_is_set.datatype)
            throw std::runtime_error("you have to specify name, address and datatype of Variable");

        return IBuilder::get_product();
    }
}
