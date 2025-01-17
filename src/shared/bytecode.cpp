#include "bytecode.hpp"

//std
#include <ranges>

namespace amasm::shared {
    size_t Bytecode::size() const {
        return _container.size();
    }

    std::byte& Bytecode::operator[](size_t idx) {
        return _container[idx];
    }
    const std::byte& Bytecode::operator[](size_t idx) const {
        return _container[idx];
    }

    byte_vector Bytecode::get(size_t length, size_t offset) const {
        return std::ranges::to<std::vector>(_container | std::views::take(offset));
    }

    void Bytecode::transform(size_t offset, size_t length, const std::function<std::byte(size_t, std::byte)>& pred) {
        for (size_t i = 0; i < length; i++)
            _container[i + offset] = pred(i, _container[i + offset]);
    }

    void Bytecode::append_sequence(const byte_vector& what) {
        const auto begin = what.data();
        _container.append_range(what);
    }
    void Bytecode::append_sequence(const Bytecode& another) {
        _container.append_range(another._container);
    }

    byte_vector& Bytecode::raw() {
        return this->_container;
    }
    const byte_vector& Bytecode::raw() const {
        return this->_container;
    }
}
