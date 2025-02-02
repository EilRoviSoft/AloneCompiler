#include "instructions.hpp"

//std
#include <stdexcept>

namespace amasm::compiler {
    //InstInfo

    const std::string& InstInfo::name() { return m_name; }
    lib::inst_code InstInfo::code() const { return m_code; }
    size_t InstInfo::min_args() const { return m_min_args; }
    size_t InstInfo::max_args() const { return m_max_args; }
    size_t InstInfo::bit_depth() const { return m_bit_depth; }

    //InstInfoBuilder

    InstInfoBuilder& InstInfoBuilder::name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::code(lib::inst_code code) {
        m_product->m_code = code;
        _status.code = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::arguments_count(size_t count) {
        m_product->m_min_args = m_product->m_max_args = count;
        _status.arguments = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::arguments_count(size_t min, size_t max) {
        m_product->m_min_args = min;
        m_product->m_max_args = max;
        _status.arguments = true;
        return *this;
    }
    InstInfoBuilder& InstInfoBuilder::bit_depth(size_t bit_depth) {
        m_product->m_bit_depth = bit_depth;
        _status.bit_depth = true;
        return *this;
    }

    InstInfo&& InstInfoBuilder::build() {
        if (!_status.name || !_status.code)
            throw std::runtime_error("you have to specify name and code of InstInfo");
        if (!_status.arguments)
            m_product->m_min_args = m_product->m_max_args = 0;
        if (!_status.bit_depth)
            m_product->m_bit_depth = 0;

        return IBuilder::build();
    }

    // ArgumentInfo

    const std::string& ArgumentInfo::name() const { return m_name; }
    ptrdiff_t ArgumentInfo::value() const { return m_value; }
    ArgumentType ArgumentInfo::type() const { return m_type; }

    // ArgumentInfoBuilder

    ArgumentInfoBuilder& ArgumentInfoBuilder::name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    ArgumentInfoBuilder& ArgumentInfoBuilder::value(ptrdiff_t value) {
        m_product->m_value = value;
        _status.value = true;
        return *this;
    }
    // if you want JumpAddress, you have to specify it manually
    ArgumentInfoBuilder& ArgumentInfoBuilder::type(ArgumentType type) {
        m_product->m_type = type;
        _status.type = true;
        return *this;
    }

    ArgumentInfo&& ArgumentInfoBuilder::build() {
        if (!_status.name && !_status.value)
            throw std::runtime_error("you have to specify name or/and of ArgumentInfo");
        if (!_status.type) {
            if (_status.name && !_status.value)
                m_product->m_type = ArgumentType::Direct;
            else if (!_status.name && _status.value)
                m_product->m_type = ArgumentType::Immediate;
            else if (_status.name && _status.value)
                m_product->m_type = ArgumentType::IndirectWithDisplacement;
        }

        return IBuilder::build();
    }

    // InstDecl

    const std::string& InstDecl::name() const { return m_name; }
    const ArgumentInfo& InstDecl::argument(size_t idx) const { return m_arguments[idx]; }

    // InstDeclBuilder

    InstDeclBuilder& InstDeclBuilder::name(std::string name) {
        m_product->m_name = std::move(name);
        _status.name = true;
        return *this;
    }
    InstDeclBuilder& InstDeclBuilder::add_argument(ArgumentInfo&& arg) {
        m_product->m_arguments.emplace_back(std::move(arg));
        return *this;
    }

    InstDecl&& InstDeclBuilder::build() {
        if (!_status.name)
            throw std::runtime_error("you have to specify name of InstDecl");

        return IBuilder::build();
    }

    // IsaFactory

    IsaFactory& IsaFactory::generate_system() {
        using B = InstInfoBuilder;
        m_product->append_range(std::initializer_list {
            B().name("halt")
               .code(0x0)
               .build(),
            B().name("fcall")
               .code(0x2)
               .arguments_count(1)
               .build(),
            B().name("ret")
               .code(0x3)
               .arguments_count(0, 1)
               .build()
        });

        return *this;
    }
    template<typename T, size_t TOffset>
    IsaFactory& IsaFactory::generate_universal_size() {
        using B = InstInfoBuilder;
        const std::string suffix = std::to_string(sizeof(T) * 8);
        m_product->append_range(std::initializer_list {
            B().name("mov" + suffix)
               .code(TOffset + 0x0)
               .arguments_count(2)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("push" + suffix)
               .code(TOffset + 0x1)
               .arguments_count(0, 1)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("pop" + suffix)
               .code(TOffset + 0x2)
               .arguments_count(0, 1)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("not" + suffix)
               .code(TOffset + 0x3)
               .arguments_count(0, 1)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("and" + suffix)
               .code(TOffset + 0x4)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("or" + suffix)
               .code(TOffset + 0x5)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("xor" + suffix)
               .code(TOffset + 0x6)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("shl" + suffix)
               .code(TOffset + 0x7)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("shr" + suffix)
               .code(TOffset + 0x8)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build()
        });

        return *this;
    }
    template<typename T, char TPostfix, size_t TOffset>
    IsaFactory& IsaFactory::generate_universal_type() {
        using B = InstInfoBuilder;
        const std::string suffix = std::to_string(sizeof(T) * 8) + TPostfix;
        m_product->append_range(std::initializer_list {
            B().name("add" + suffix)
               .code(TOffset + 0x0)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("sub" + suffix)
               .code(TOffset + 0x1)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("mul" + suffix)
               .code(TOffset + 0x2)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("div" + suffix)
               .code(TOffset + 0x3)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("mod" + suffix)
               .code(TOffset + 0x4)
               .arguments_count(2, 3)
               .bit_depth(sizeof(T) * 8)
               .build(),
            B().name("cmp" + suffix)
               .code(TOffset + 0x5)
               .arguments_count(2)
               .bit_depth(sizeof(T) * 8)
               .build()
        });

        return *this;
    }
    template<typename T, char TPostfix, size_t TOffset>
    IsaFactory& IsaFactory::generate_signed_type() {
        using B = InstInfoBuilder;
        m_product->append_range(generate_universal_type<T, TPostfix, TOffset>());
        m_product->emplace_back(
            B().name("neg" + std::to_string(sizeof(T) * 8) + TPostfix)
               .code(TOffset + 0x8)
               .arguments_count(1, 2)
               .bit_depth(sizeof(T) * 8)
               .build()
        );

        return *this;
    }

    IsaFactory& IsaFactory::generate_info() {
        // [halt, fcall, ret]
        generate_system();

        // [mov push pop not and or xor shl shr] for [8 16 32 64]
        generate_universal_size<uint8_t, 0x100>();
        generate_universal_size<uint16_t, 0x110>();
        generate_universal_size<uint32_t, 0x120>();
        generate_universal_size<uint64_t, 0x130>();

        // [add sub mul div mod cmp] for [8u 16u 32u 64u]
        generate_universal_type<uint8_t, 'u', 0x140>();
        generate_universal_type<uint16_t, 'u', 0x150>();
        generate_universal_type<uint32_t, 'u', 0x160>();
        generate_universal_type<uint64_t, 'u', 0x170>();

        // [add sub mul div mod cmp neg] for [8i 16i 32i 64i]
        generate_universal_type<uint8_t, 'i', 0x180>();
        generate_universal_type<uint16_t, 'i', 0x190>();
        generate_universal_type<uint32_t, 'i', 0x1A0>();
        generate_universal_type<uint64_t, 'i', 0x1B0>();

        return *this;
    }
}
