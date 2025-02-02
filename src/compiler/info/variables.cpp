#include "variables.hpp"

namespace amasm::compiler {
    // Variable

    lib::address Variable::address() const { return m_address; }
    const Datatype& Variable::datatype() const { return *m_datatype; }

    // VariableBuilder

    VariableBuilder& VariableBuilder::name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    VariableBuilder& VariableBuilder::address(lib::address address) {
        m_product->m_address = address;
        _status.address = true;
        return *this;
    }
    VariableBuilder& VariableBuilder::datatype(const Datatype& datatype) {
        m_product->m_datatype = &datatype;
        _status.datatype = true;
        return *this;
    }

    Variable&& VariableBuilder::build() {
        if (!_status.name || !_status.address || !_status.datatype)
            throw std::runtime_error("you have to specify name, address and datatype of Variable");
        return IBuilder::build();
    }
}
