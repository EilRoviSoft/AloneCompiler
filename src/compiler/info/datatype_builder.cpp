#include "compiler/info/datatype_builder.hpp"

//std
#include <stdexcept>

//library
#include "library/general_functions.hpp"

namespace amasm::compiler {
    // PoleDeclBuilder

    PoleDeclBuilder& PoleDeclBuilder::set_name(std::string name) {
        m_product->m_name = std::move(name);
        _is_set.name = true;
        return *this;
    }
    PoleDeclBuilder& PoleDeclBuilder::set_datatype(const Datatype& type) {
        m_product->m_datatype = &type;
        _is_set.type = true;
        return *this;
    }
    PoleDeclBuilder& PoleDeclBuilder::set_offset(ptrdiff_t offset) {
        m_product->m_offset = offset;
        return *this;
    }

    PoleDecl&& PoleDeclBuilder::get_product() {
        if (!_is_set.name || !_is_set.type)
            throw std::runtime_error("you have to specify name and type of PoleDecl");

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
    DatatypeBuilder& DatatypeBuilder::add_pole(std::string name, const Datatype& type) {
        ptrdiff_t offset;
        if (m_product->m_poles.empty()) {
            const auto& last = m_product->m_poles.back();
            offset = last.offset() + last.datatype().size();
        } else
            offset = 0;

        auto pole = PoleDeclBuilder()
            .set_name(std::move(name))
            .set_datatype(type)
            .set_offset(offset)
            .get_product();

        return add_pole(std::move(pole));
    }
    DatatypeBuilder& DatatypeBuilder::add_pole(PoleDecl&& pole) {
        auto hashed_key = lib::hash_string(pole.name());

        if (m_product->m_poles_search.contains(hashed_key))
            return *this;

        m_product->m_size += pole.datatype().size();
        const auto& it = m_product->m_poles.emplace_back(std::move(pole));
        m_product->m_poles_search.emplace(hashed_key, it);

        return *this;
    }

    Datatype&& DatatypeBuilder::get_product() {
        if (!_is_set.name)
            throw std::runtime_error("you have to specify name and size of Datatype");

        return IBuilder::get_product();
    }
}
