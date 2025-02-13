#include "compiler/info/inst_info_factory.hpp"

//compiler_info
#include "compiler/info/instruction_builder.hpp"

namespace amasm::compiler {
    // generators

    std::list<InstInfo> generate_system() {
        return {
            InstInfoBuilder()
            .set_name("halt")
            .set_code(0x0)
            .get_product(),
            InstInfoBuilder()
            .set_name("ncall")
            .set_code(0x1)
            .set_arguments_count(1)
            .get_product(),
            InstInfoBuilder()
            .set_name("fcall")
            .set_code(0x2)
            .set_arguments_count(1)
            .get_product(),
            InstInfoBuilder()
            .set_name("ret")
            .set_code(0x3)
            .set_arguments_count(0, 1)
            .get_product()
        };
    }
    template<typename T, size_t TOffset>
    std::list<InstInfo> generate_universal_size() {
        const std::string suffix = std::to_string(sizeof(T) * 8);
        return {
            InstInfoBuilder()
            .set_name("mov" + suffix)
            .set_code(TOffset + 0x0)
            .set_arguments_count(2)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("push" + suffix)
            .set_code(TOffset + 0x1)
            .set_arguments_count(0, 1)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("pop" + suffix)
            .set_code(TOffset + 0x2)
            .set_arguments_count(0, 1)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("not" + suffix)
            .set_code(TOffset + 0x3)
            .set_arguments_count(0, 1)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("and" + suffix)
            .set_code(TOffset + 0x4)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("or" + suffix)
            .set_code(TOffset + 0x5)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("xor" + suffix)
            .set_code(TOffset + 0x6)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("shl" + suffix)
            .set_code(TOffset + 0x7)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("shr" + suffix)
            .set_code(TOffset + 0x8)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product()
        };
    }
    template<typename T, char TPostfix, size_t TOffset>
    std::list<InstInfo> generate_universal_type() {
        const std::string suffix = std::to_string(sizeof(T) * 8) + TPostfix;
        return {
            InstInfoBuilder()
            .set_name("add" + suffix)
            .set_code(TOffset + 0x0)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("sub" + suffix)
            .set_code(TOffset + 0x1)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("mul" + suffix)
            .set_code(TOffset + 0x2)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("div" + suffix)
            .set_code(TOffset + 0x3)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("mod" + suffix)
            .set_code(TOffset + 0x4)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
            InstInfoBuilder()
            .set_name("cmp" + suffix)
            .set_code(TOffset + 0x5)
            .set_arguments_count(2)
            .set_bit_depth(sizeof(T) * 8)
            .get_product()
        };
    }
    template<typename T, char TPostfix, size_t TOffset>
    std::list<InstInfo> generate_signed_type() {
        std::list<InstInfo> result;
        result.append_range(generate_universal_type<T, TPostfix, TOffset>());
        result.emplace_back(
            InstInfoBuilder()
            .set_name("neg" + std::to_string(sizeof(T) * 8) + TPostfix)
            .set_code(TOffset + 0x8)
            .set_arguments_count(1, 2)
            .set_bit_depth(sizeof(T) * 8)
            .get_product()
        );
        return result;
    }

    // InstInfoFactory

    InstInfoFactory& InstInfoFactory::generate_info() {
        // [halt, fcall, ret]
        m_product->append_range(generate_system());

        // [mov push pop not and or xor shl shr] for [8 16 32 64]
        m_product->append_range(generate_universal_size<uint8_t, 0x100>());
        m_product->append_range(generate_universal_size<uint16_t, 0x110>());
        m_product->append_range(generate_universal_size<uint32_t, 0x120>());
        m_product->append_range(generate_universal_size<uint64_t, 0x130>());

        // [add sub mul div mod cmp] for [8u 16u 32u 64u]
        m_product->append_range(generate_universal_type<uint8_t, 'u', 0x140>());
        m_product->append_range(generate_universal_type<uint16_t, 'u', 0x150>());
        m_product->append_range(generate_universal_type<uint32_t, 'u', 0x160>());
        m_product->append_range(generate_universal_type<uint64_t, 'u', 0x170>());

        // [add sub mul div mod cmp neg] for [8i 16i 32i 64i]
        m_product->append_range(generate_universal_type<uint8_t, 'i', 0x180>());
        m_product->append_range(generate_universal_type<uint16_t, 'i', 0x190>());
        m_product->append_range(generate_universal_type<uint32_t, 'i', 0x1A0>());
        m_product->append_range(generate_universal_type<uint64_t, 'i', 0x1B0>());

        return *this;
    }
}
