#pragma once

//std
#include <cstddef>
#include <functional>
#include <vector>

//lib
#include "library/types.hpp"

namespace amasm::lib {
    class Bytecode {
    public:
        size_t size() const {
            return _container.size();
        }

        std::byte& operator[](size_t idx) {
            return _container[idx];
        }
        const std::byte& operator[](size_t idx) const {
            return _container[idx];
        }

        template<typename T = machine_word>
        void set(const T& val, size_t offset) {
            for (size_t i = 0; i < sizeof(T); i++)
                _container[i + offset] = std::byte(val >> (i * 8) & 0xff);
        }

        void transform(size_t offset, size_t length, const std::function<std::byte(size_t, std::byte)>& pred) {
            for (size_t i = 0; i < length; i++)
                _container[i + offset] = pred(i, _container[i + offset]);
        }

        void append_sequence(const byte_vector& what) {
            const auto begin = what.data();
            _container.append_range(what);
        }
        void append_sequence(const Bytecode& another) {
            _container.append_range(another._container);
        }
        template<typename T = machine_word>
        void append_value(const T& value, size_t size = sizeof(T)) {
            const auto as_bytes = reinterpret_cast<const std::byte*>(&value);
            ptrdiff_t i = 0;

            _container.reserve(_container.size() + size);

            if (sizeof(T) < size)
                while (i++ < size - sizeof(T)) {
                    _container.emplace_back((std::byte) 0);
                    i++;
                }
            while (i < size) {
                _container.emplace_back(as_bytes[i]);
                i++;
            }
        }

        const std::byte* data() {
            return _container.data();
        }

    private:
        byte_vector _container;
    };
}
