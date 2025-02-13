#include "compiler/info/datatype_builder.hpp"

//std
#include <stdexcept>

//library
#include "library/general_functions.hpp"

//compiler_info
#include "compiler/info/variable_builder.hpp"

namespace amasm::compiler {
    // PoleBuilder

    PoleBuilder& PoleBuilder::set_name(std::string name) {
        m_product->m_name = std::move(name);
        _is_set.name = true;
        return *this;
    }
    PoleBuilder& PoleBuilder::set_datatype(const Datatype& datatype) {
        m_product->m_datatype = &datatype;
        _is_set.datatype = true;
        return *this;
    }

    bool PoleBuilder::is_built() const {
        return _is_set.name && _is_set.datatype;
    }
    Pole&& PoleBuilder::get_product() {
        if (!is_built())
            throw std::runtime_error("you have to specify name and datatype of Pole");

        return IBuilder::get_product();
    }

    // DatatypeBuilder

    DatatypeBuilder& DatatypeBuilder::set_name(std::string name) {
        m_product->m_name = std::move(name);
        _is_set.name = true;
        return *this;
    }
    DatatypeBuilder& DatatypeBuilder::set_size(size_t size) {
        m_product->m_size = size;
        return *this;
    }
    DatatypeBuilder& DatatypeBuilder::add_pole(Pole&& pole) {
        auto hashed_key = lib::hash_string(pole.name());

        if (m_product->m_poles_search.contains(hashed_key))
            return *this;

        if (!m_product->m_poles.empty()) {
            const auto& last = m_product->m_poles.back();
            pole.m_offset = last.offset() + last.datatype().size();
        } else
            pole.m_offset = 0;

        m_product->m_size += pole.datatype().size();
        const auto& it = m_product->m_poles.emplace_back(std::move(pole));
        m_product->m_poles_search.emplace(hashed_key, &it);

        return *this;
    }

    bool DatatypeBuilder::is_built() const {
        return _is_set.name;
    }

    Datatype&& DatatypeBuilder::get_product() {
        if (!is_built())
            throw std::runtime_error("you have to specify name and size of Datatype");

        return IBuilder::get_product();
    }
}
