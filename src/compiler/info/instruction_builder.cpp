#include "compiler/info/instruction_builder.hpp"

//std
#include <stdexcept>
#include <utility>

namespace amasm::compiler {
    //InstInfoBuilder

    InstInfoBuilder& InstInfoBuilder::set_name(std::string name) {
        m_product->m_name = std::move(name);
        _is_set.name = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::set_code(lib::inst_code code) {
        m_product->m_code = code;
        _is_set.code = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::set_arguments_count(size_t count) {
        m_product->m_min_args = count;
        m_product->m_max_args = count;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::set_arguments_count(size_t min, size_t max) {
        m_product->m_min_args = min;
        m_product->m_max_args = max;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::set_bit_depth(size_t bit_depth) {
        m_product->m_bit_depth = bit_depth;
        return *this;
    }

    bool InstInfoBuilder::is_built() const {
        return _is_set.name
            && _is_set.code;
    }

    InstInfo&& InstInfoBuilder::get_product() {
        if (!is_built())
            throw std::runtime_error("you have to specify name and code of InstInfo");

        return IBuilder::get_product();
    }

    // InstDeclBuilder

    InstDeclBuilder& InstDeclBuilder::set_info(const InstInfo& info) {
        m_product->m_info = &info;
        _is_set.info = true;
        return *this;
    }
    InstDeclBuilder& InstDeclBuilder::add_argument(address_info&& arg) {
        m_product->m_arguments.emplace_back(std::move(arg));
        return *this;
    }

    bool InstDeclBuilder::is_built() const {
        return _is_set.info;
    }
    InstDecl&& InstDeclBuilder::get_product() {
        if (!is_built())
            throw std::runtime_error("you have to specify info of InstDecl");

        return IBuilder::get_product();
    }
}
