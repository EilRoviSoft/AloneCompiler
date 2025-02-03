#pragma once

//std
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace amasm::compiler {
    class IScopeElement {
        friend class ScopeContainer;
        friend class ScopeProxy;

    public:
        virtual ~IScopeElement() = default;

        const std::string& name() const;
        size_t type_id() const;

        virtual size_t hash() const;

    protected:
        size_t m_type_id;
        std::string m_name;

        explicit IScopeElement(size_t type);
    };
    using ScopeElement = std::shared_ptr<IScopeElement>;

    template<typename T>
    concept scope_element_t = requires {
        std::is_base_of_v<IScopeElement, T>;
    };

    using layer = std::unordered_map<size_t, const IScopeElement*>;
}
