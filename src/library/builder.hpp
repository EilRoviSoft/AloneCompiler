#pragma once

//std
#include <memory>

namespace amasm::lib {
    template<typename T>
    class IBuilder {
    public:
        IBuilder() : m_product(new T) {}
        virtual ~IBuilder() = default;

        virtual T&& get_product() { return std::move(*m_product); }

    protected:
        std::unique_ptr<T> m_product;
    };
}
