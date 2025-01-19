#pragma once

//std
#include <cstddef>
#include <functional>
#include <vector>

//shared
#include "shared/types.hpp"

namespace amasm::shared {
    class Bytecode {
    public:
        size_t size() const;

        std::byte& operator[](size_t idx);
        const std::byte& operator[](size_t idx) const;

        template<typename T = machine_word>
        void set(const T& val, size_t offset) {
            for (size_t i = 0; i < sizeof(T); i++)
                _container[i + offset] = std::byte(val >> (i * 8) & 0xff);
        }

        void transform(size_t offset, size_t length, const std::function<std::byte(size_t, std::byte)>& pred);

        void append_sequence(const byte_vector& what);
        void append_sequence(const Bytecode& another);
        // size <= sizeof(T)
        template<typename T = machine_word>
        void append_value(const T& value, size_t size = sizeof(T)) {
            const auto as_bytes = reinterpret_cast<const std::byte*>(&value);
            _container.append_range(std::initializer_list(as_bytes, as_bytes + size));
        }

        const std::byte* data();

    private:
        byte_vector _container;
    };
}
