#pragma once

//std
#include <memory>
#include <stdexcept>
#include <string>

//library
#include "library/logger.hpp"

namespace amasm::lib {
    template<typename T>
    class IBuilder {
    public:
        IBuilder() : m_product(new T) {}
        virtual ~IBuilder() = default;

        IBuilder(IBuilder&& another) noexcept {
            m_product.reset(another.m_product.release());
        }

        virtual bool is_built() const = 0;

        T&& get_product() {
            if (!is_built()) {
                Logger::channel(Logger::Error) << __FILE__ << ": " << get_error_message() << std::endl;
                throw std::invalid_argument("see logger for detailed information");
            }

            return std::move(*m_product);
        }

    protected:
        std::unique_ptr<T> m_product;

        virtual const char* get_error_message() const = 0;
    };
}
