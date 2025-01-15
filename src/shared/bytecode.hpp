#pragma once

//std
#include <cstddef>
#include <deque>
#include <functional>
#include <ranges>

//shared
#include "shared/types.hpp"

namespace alone::shared {
    class Bytecode {
    public:
        size_t size() const;

        std::byte& operator[](size_t idx);
        const std::byte& operator[](size_t idx) const;

        data_sequence_t get(size_t length, size_t offset) const;
        template<typename T>
        void set(const T& val, size_t offset) {
            for (size_t i = 0; i < sizeof(T); i++)
                _container[i + offset] = std::byte((val & 0xff << i * 8) >> i * 8);
        }

        void transform(size_t offset, size_t length, const std::function<std::byte(std::byte)>& pred);

        void append_sequence(const data_sequence_t& what);
        template<typename T>
        void append_value(const T& value, size_t size = sizeof(T)) {
            const auto as_bytes = reinterpret_cast<const std::byte*>(&value);
            _container.append_range(std::initializer_list(as_bytes, as_bytes + size));
        }

    private:
        std::deque<std::byte> _container;
    };
}
