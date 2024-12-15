#include "bytecode.hpp"

namespace alone::shared {
    std::byte& Bytecode::operator[](const size_t& idx) {
        return m_container[idx];
    }
    const std::byte& Bytecode::operator[](const size_t& idx) const {
        return m_container[idx];
    }
}
