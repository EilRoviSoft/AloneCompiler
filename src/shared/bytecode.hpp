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
        using mask_predicate_t = std::function<std::byte(const std::byte&, const std::byte&)>;

        std::byte& operator[](const size_t& idx);
        const std::byte& operator[](const size_t& idx) const;

        data_sequence_t get(const size_t& length, const size_t& offset) const {
            data_sequence_t result;

            result.reserve(length);
            for (size_t i = 0; i < length; i++)
                result.emplace_back(m_container[i + offset]);

            return std::ranges::to<std::vector>(m_container | std::views::take(offset));
        }
        template<typename T>
        void set(const T& val, const size_t& offset) {
            for (size_t i = 0; i < sizeof(T); i++)
                m_container[i + offset] = std::byte((val & 0xff << i * 8) >> i * 8);
        }

        void apply_mask(const std::vector<std::byte>& mask, const mask_predicate_t& pred, const size_t& offset = 0) {
            const auto as_bytes = reinterpret_cast<const std::byte*>(&mask);
            for (size_t i = 0; i < mask.size(); i++)
                m_container[i + offset] = pred(m_container[i + offset], as_bytes[i]);
        }

    protected:
        std::deque<std::byte> m_container;
    };
}
