#pragma once

//std
#include <string>
#include <vector>

//library
#include "library/general_functions.hpp"
#include "library/types.hpp"

namespace amasm::compiler {
    class InstInfo {
        friend class InstInfoBuilder;

    public:
        const std::string& name() const { return m_name; }
        lib::inst_code code() const { return m_code; }
        size_t min_args() const { return m_min_args; }
        size_t max_args() const { return m_max_args; }
        size_t bit_depth() const { return m_bit_depth; }

        size_t hash() const { return lib::hash_string(m_name); }

    protected:
        std::string m_name;
        lib::inst_code m_code = 0;
        size_t m_min_args = 0, m_max_args = 0, m_bit_depth = 0;
    };

    class ArgumentInfo {
        friend class ArgumentInfoBuilder;

    public:
        const std::string& name() const { return m_name; }
        ptrdiff_t value() const { return m_value; }
        ArgumentType type() const { return m_type; }

    protected:
        std::string m_name;
        ptrdiff_t m_value = 0;
        ArgumentType m_type;
    };

    class InstDecl {
        friend class InstDeclBuilder;

    public:
        const std::string& name() const { return m_name; }
        const ArgumentInfo& argument(size_t idx) const { return m_arguments[idx]; }

    protected:
        std::string m_name;
        std::vector<ArgumentInfo> m_arguments;
    };
    using ComposedInst = std::tuple<size_t, InstDecl>;
}
