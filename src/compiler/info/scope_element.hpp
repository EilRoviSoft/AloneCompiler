#pragma once

//std
#include <concepts>
#include <memory>
#include <string>

namespace amasm::compiler {
    class IScopeElement {
        friend class Scope;
        friend class ScopeProxy;

    public:
        virtual ~IScopeElement() = default;

        const std::string& name() const;

        virtual size_t hash() const;

    protected:
        std::string m_name;

        enum class Type : uint8_t {
            None = 0,
            Datatype,
            Function,
            Variable
        };

        explicit IScopeElement(Type type);

    private:
        Type _type;
    };
    using ScopeElement = std::shared_ptr<IScopeElement>;

    template<typename T>
    concept scope_element_t = requires {
        std::derived_from<T, IScopeElement>;
    };
}
