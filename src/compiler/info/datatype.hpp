#pragma once

//std
#include <list>
#include <string>
#include <unordered_map>

//library
#include "library/general_functions.hpp"

//compiler_info
#include "compiler/info/scope_element.hpp"

namespace amasm::compiler {
    class Datatype;

    class PoleDecl {
        friend class PoleDeclBuilder;
        friend class ScopeContainer;

    public:
        const std::string& name() const { return m_name; }
        const Datatype& datatype() const { return *m_datatype; }
        ptrdiff_t offset() const { return m_offset; }

    protected:
        std::string m_name;
        const Datatype* m_datatype = nullptr;
        ptrdiff_t m_offset = 0;
    };

    class Datatype : public IScopeElement {
        friend class DatatypeBuilder;
        friend class ScopeContainer;

    public:
        Datatype() : IScopeElement(1) {}

        size_t size() const { return m_size; }
        const PoleDecl& pole(const std::string& pole_name) const {
            size_t hash = lib::hash_string(pole_name);
            return m_poles_search.at(hash);
        }

        ScopeElement clone() const override {
            return std::make_shared<Datatype>(*this);
        }

    protected:
        size_t m_size = 0;
        std::list<PoleDecl> m_poles;
        std::unordered_map<size_t, PoleDecl> m_poles_search;
    };
}
