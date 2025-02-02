#include "instruction_builder.hpp"

//std
#include <stdexcept>

namespace amasm::compiler {
    //InstInfoBuilder

    InstInfoBuilder& InstInfoBuilder::name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::code(lib::inst_code code) {
        m_product->m_code = code;
        _status.code = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::arguments_count(size_t count) {
        m_product->m_min_args = m_product->m_max_args = count;
        _status.arguments = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::arguments_count(size_t min, size_t max) {
        m_product->m_min_args = min;
        m_product->m_max_args = max;
        _status.arguments = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::bit_depth(size_t bit_depth) {
        m_product->m_bit_depth = bit_depth;
        _status.bit_depth = true;
        return *this;
    }

    InstInfo&& InstInfoBuilder::build() {
        if (!_status.name || !_status.code)
            throw std::runtime_error("you have to specify name and code of InstInfo");
        if (!_status.arguments)
            m_product->m_min_args = m_product->m_max_args = 0;
        if (!_status.bit_depth)
            m_product->m_bit_depth = 0;

        return IBuilder::build();
    }

    // ArgumentInfoBuilder

    ArgumentInfoBuilder& ArgumentInfoBuilder::name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    ArgumentInfoBuilder& ArgumentInfoBuilder::value(ptrdiff_t value) {
        m_product->m_value = value;
        _status.value = true;
        return *this;
    }
    // if you want JumpAddress, you have to specify it manually
    ArgumentInfoBuilder& ArgumentInfoBuilder::type(ArgumentType type) {
        m_product->m_type = type;
        _status.type = true;
        return *this;
    }

    ArgumentInfo&& ArgumentInfoBuilder::build() {
        if (!_status.name && !_status.value)
            throw std::runtime_error("you have to specify name or/and of ArgumentInfo");
        if (!_status.type) {
            if (_status.name && !_status.value)
                m_product->m_type = ArgumentType::Direct;
            else if (!_status.name && _status.value)
                m_product->m_type = ArgumentType::Immediate;
            else if (_status.name && _status.value)
                m_product->m_type = ArgumentType::IndirectWithDisplacement;
        }

        return IBuilder::build();
    }

    // InstDeclBuilder

    InstDeclBuilder& InstDeclBuilder::name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    InstDeclBuilder& InstDeclBuilder::add_argument(ArgumentInfo&& arg) {
        m_product->m_arguments.emplace_back(std::move(arg));
        return *this;
    }

    InstDecl&& InstDeclBuilder::build() {
        if (!_status.name)
            throw std::runtime_error("you have to specify name of InstDecl");

        return IBuilder::build();
    }
}
