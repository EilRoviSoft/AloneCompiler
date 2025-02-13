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

    class Pole {
        friend class PoleBuilder;
        friend class DatatypeBuilder;
        friend class ScopeContainer;

    public:
        std::string name() const { return m_name; }
        ptrdiff_t offset() const { return m_offset; }
        const Datatype& datatype() const { return *m_datatype; }

    protected:
        std::string m_name;
        ptrdiff_t m_offset = 0;
        const Datatype* m_datatype = nullptr;
    };

    class Datatype : public IScopeElement {
        friend class DatatypeBuilder;
        friend class ScopeContainer;

    public:
        Datatype() : IScopeElement(1) {}

        size_t size() const { return m_size; }
        auto pole(const std::string& pole_name) const {
            size_t hash = lib::hash_string(pole_name);
            return *m_poles_search.at(hash);
        }

        ScopeElement clone() const override {
            return std::make_shared<Datatype>(*this);
        }

    protected:
        size_t m_size = 0;
        std::list<Pole> m_poles;
        std::unordered_map<size_t, const Pole*> m_poles_search;
    };
}
