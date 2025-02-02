#pragma once

//std
#include <memory>

namespace amasm::lib {
    template<typename T>
    class IFactory {
    public:
        IFactory() : m_product(new T) {}
        virtual ~IFactory() = default;

        virtual T&& make() { return std::move(*m_product); }

    protected:
        std::unique_ptr<T> m_product;
    };
}
