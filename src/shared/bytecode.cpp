#include "bytecode.hpp"

namespace alone::shared {
    size_t Bytecode::size() const {
        return _container.size();
    }

    std::byte& Bytecode::operator[](size_t idx) {
        return _container[idx];
    }
    const std::byte& Bytecode::operator[](size_t idx) const {
        return _container[idx];
    }

    data_sequence_t Bytecode::get(size_t length, size_t offset) const {
        data_sequence_t result;

        result.reserve(length);
        for (size_t i = 0; i < length; i++)
            result.emplace_back(_container[i + offset]);

        return std::ranges::to<std::vector>(_container | std::views::take(offset));
    }

    void Bytecode::transform(size_t offset, size_t length, const std::function<std::byte(size_t, std::byte)>& pred) {
        for (size_t i = 0; i < length; i++)
            _container[i + offset] = pred(i, _container[i + offset]);
    }

    void Bytecode::append_sequence(const data_sequence_t& what) {
        const auto begin = what.data();
        _container.append_range(std::initializer_list(begin, begin + what.size()));
    }

    std::vector<std::byte> Bytecode::to_vector() {
        return std::ranges::to<std::vector>(_container);
    }
}
