#include "compiler/info/constant_builder.hpp"

namespace amasm::compiler {
    ConstantBuilder& ConstantBuilder::set_name(std::string name) {
        m_product->m_name = std::move(name);
        _is_set.name = true;
        return *this;
    }
    ConstantBuilder& ConstantBuilder::set_address(lib::address address) {
        m_product->m_address = {
            .name = "",
            .abs_value = address,
            .sign_value = 0,
            .type = AddressType::Fixed
        };
        _is_set.address = true;
        return *this;
    }
    ConstantBuilder& ConstantBuilder::set_datatype(const Datatype& datatype) {
        m_product->m_datatype = &datatype;
        _is_set.datatype = true;
        return *this;
    }
    ConstantBuilder& ConstantBuilder::set_data(lib::byte_vector&& data) {
        m_product->m_data = std::move(data);
        _is_set.datatype = true;
        return *this;
    }

    bool ConstantBuilder::is_built() const {
        return _is_set.name
            && _is_set.address
            && _is_set.datatype
            && _is_set.data;
    }

    Constant&& ConstantBuilder::get_product() {
        if (!is_built())
            throw std::runtime_error("you have to specify name, address, datatype and data of Constant");

        return IBuilder::get_product();
    }
}
