#include "datatype_builder.hpp"

//std
#include <stdexcept>

//library
#include "library/general_functions.hpp"

namespace amasm::compiler {
    DatatypeBuilder& DatatypeBuilder::set_name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    DatatypeBuilder& DatatypeBuilder::set_size(size_t size) {
        m_product->m_size = size;
        _status.size = true;
        return *this;
    }
    DatatypeBuilder& DatatypeBuilder::add_pole(std::string name, const Datatype& type) {
        auto hashed_key = lib::hash_string(name);

        if (m_product->m_poles_search.contains(hashed_key))
            return *this;

        bool has_poles = m_product->m_poles.empty();
        ptrdiff_t offset = has_poles ? 0 : m_product->m_poles.back().offset + m_product->m_poles.back().type.m_size;
        const auto& it = m_product->m_poles.emplace_back(std::move(name), type, offset);
        m_product->m_poles_search.emplace(hashed_key, it);

        return *this;
    }

    Datatype&& DatatypeBuilder::get_product() {
        if (!_status.name || !_status.size)
            throw std::runtime_error("you have to specify name and size of Datatype");

        return IBuilder::get_product();
    }
}
