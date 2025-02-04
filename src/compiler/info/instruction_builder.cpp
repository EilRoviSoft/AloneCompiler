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
        m_product->m_min_args = count;
        m_product->m_max_args = count;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::arguments_count(size_t min, size_t max) {
        m_product->m_min_args = min;
        m_product->m_max_args = max;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::bit_depth(size_t bit_depth) {
        m_product->m_bit_depth = bit_depth;
        return *this;
    }

    InstInfo&& InstInfoBuilder::get_product() {
        if (!_status.name || !_status.code)
            throw std::runtime_error("you have to specify name and code of InstInfo");

        return IBuilder::get_product();
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

    ArgumentInfo&& ArgumentInfoBuilder::get_product() {
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

        return IBuilder::get_product();
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

    InstDecl&& InstDeclBuilder::get_product() {
        if (!_status.name)
            throw std::runtime_error("you have to specify name of InstDecl");

        return IBuilder::get_product();
    }
}
