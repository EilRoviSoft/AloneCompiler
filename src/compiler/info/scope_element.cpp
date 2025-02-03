#include "scope_element.hpp"

//library
#include "library/general_functions.hpp"

namespace amasm::compiler {
    IScopeElement::IScopeElement(size_t type) :
        m_type_id(type) {
    }

    const std::string& IScopeElement::name() const { return m_name; }
    size_t IScopeElement::type_id() const { return m_type_id; }

    size_t IScopeElement::hash() const { return lib::hash_string(m_name); }
}
