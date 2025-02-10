#pragma once

//std
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace amasm::compiler {
    using ScopeElement = std::shared_ptr<class IScopeElement>;
    class IScopeElement {
        friend class ScopeContainer;
        friend class ScopeProxy;
        friend class ConstScopeProxy;

    public:
        virtual ~IScopeElement() = default;

        const std::string& name() const;
        size_t type_id() const;

        virtual size_t hash() const;
        virtual ScopeElement clone() const = 0;

    protected:
        size_t m_type_id;
        std::string m_name;

        explicit IScopeElement(size_t type);
    };

    template<typename T>
    concept scope_element_t = requires {
        std::is_base_of_v<IScopeElement, T>;
    };

    using layer = std::unordered_map<size_t, const IScopeElement*>;
}
