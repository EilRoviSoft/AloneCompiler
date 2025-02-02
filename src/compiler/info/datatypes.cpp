#include "datatypes.hpp"

//std
#include <stdexcept>

//library
#include "library/general_functions.hpp"

namespace amasm::compiler {
    //Datatype

    size_t Datatype::size() const { return m_size; }
    const Datatype::pole_info& Datatype::pole(const std::string& pole_name) const {
        size_t hash = lib::hash_string(pole_name);
        return m_poles_search.at(hash);
    }

    // DatatypeBuilder

    DatatypeBuilder& DatatypeBuilder::name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    DatatypeBuilder& DatatypeBuilder::size(size_t size) {
        m_product->m_size = size;
        _status.size = true;
        return *this;
    }
    DatatypeBuilder& DatatypeBuilder::add_pole(std::string name, const Datatype& type) {
        size_t hash = lib::hash_string(name);
        bool contains = m_product->m_poles_search.contains(hash);

        if (!contains) {
            bool has_poles = m_product->m_poles.empty();
            ptrdiff_t offset = has_poles ? 0 : m_product->m_poles.back().offset + m_product->m_poles.back().type.m_size;
            const auto& it = m_product->m_poles.emplace_back(std::move(name), type, offset);
            m_product->m_poles_search.emplace(hash, it);
        }

        return *this;
    }

    Datatype&& DatatypeBuilder::build() {
        if (!_status.name || !_status.size)
            throw std::runtime_error("you have to specify name and size of Datatype");

        return IBuilder::build();
    }
}
