#pragma once

//std
#include <string>

//compiler_info
#include "compiler/info/scope_info.hpp"

//library
#include "library/builder.hpp"
#include "library/factory.hpp"
#include "library/types.hpp"

namespace amasm::compiler {
    class InstInfo {
        friend class ScopeProxy;
        friend class InstInfoBuilder;

    public:
        InstInfo(const InstInfo&) = default;
        InstInfo(InstInfo&&) = default;

        const std::string& name();
        lib::inst_code code() const;
        size_t min_args() const;
        size_t max_args() const;
        size_t bit_depth() const;

    protected:
        std::string m_name;
        lib::inst_code m_code;
        size_t m_min_args, m_max_args, m_bit_depth;

    private:
        InstInfo() = default;
    };

    class InstInfoBuilder : public lib::IBuilder<InstInfo> {
    public:
        InstInfoBuilder& name(std::string name);
        InstInfoBuilder& code(lib::inst_code code);
        InstInfoBuilder& arguments_count(size_t count);
        InstInfoBuilder& arguments_count(size_t min, size_t max);
        InstInfoBuilder& bit_depth(size_t bit_depth);

        InstInfo&& build() override;

    private:
        struct {
            bool name      : 1 = false;
            bool code      : 1 = false;
            bool arguments : 1 = false;
            bool bit_depth : 1 = false;
        } _status;
    };

    class ArgumentInfo {
        friend class ArgumentInfoBuilder;

    public:
        const std::string& name() const;
        ptrdiff_t value() const;
        ArgumentType type() const;

    protected:
        std::string m_name;
        ptrdiff_t m_value;
        ArgumentType m_type;

    private:
        ArgumentInfo() = default;
    };

    class ArgumentInfoBuilder : public lib::IBuilder<ArgumentInfo> {
    public:
        ArgumentInfoBuilder& name(std::string name);
        ArgumentInfoBuilder& value(ptrdiff_t value);
        ArgumentInfoBuilder& type(ArgumentType type);

        ArgumentInfo&& build() override;

    private:
        struct {
            bool name  : 1 = false;
            bool value : 1 = false;
            bool type  : 1 = false;
        } _status;
    };

    class InstDecl {
        friend class InstDeclBuilder;

    public:
        const std::string& name() const;
        const ArgumentInfo& argument(size_t idx) const;

    protected:
        std::string m_name;
        std::vector<ArgumentInfo> m_arguments;
    };
    using ComposedInst = std::tuple<size_t, InstDecl>;

    class InstDeclBuilder : public lib::IBuilder<InstDecl> {
    public:
        InstDeclBuilder& name(std::string name);
        InstDeclBuilder& add_argument(ArgumentInfo&& arg);

        InstDecl&& build() override;

    private:
        struct {
            bool name : 1 = false;
        } _status;
    };

    class IsaFactory : public lib::IFactory<std::list<InstInfo>> {
    public:
        IsaFactory& generate_system();
        template<typename T, size_t TOffset>
        IsaFactory& generate_universal_size();
        template<typename T, char TPostfix, size_t TOffset>
        IsaFactory& generate_universal_type();
        template<typename T, char TPostfix, size_t TOffset>
        IsaFactory& generate_signed_type();

        IsaFactory& generate_info();
    };
}
