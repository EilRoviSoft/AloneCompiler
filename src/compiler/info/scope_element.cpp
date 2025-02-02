#include "scope_element.hpp"

//library
#include "library/general_functions.hpp"

namespace amasm::compiler {
    IScopeElement::IScopeElement(Type type) :
        _type(type) {
    }

    const std::string& IScopeElement::name() const { return m_name; }

    size_t IScopeElement::hash() const { return lib::hash_string(m_name); }
}
