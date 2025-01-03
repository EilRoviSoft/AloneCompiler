#include "instructions.hpp"

namespace alone::amasm {
    std::list<inst_info_t> generate_system_isa_info() {
        std::list<inst_info_t> result;

        result.append_range(std::initializer_list {
            inst_info_t {
                .name = "halt",
                .code = 0x0,
                .min_args = 0,
                .max_args = 0,
                .bid_depth = 0
            },
            inst_info_t {
                .name = "fcall",
                .code = 0x2,
                .min_args = 1,
                .max_args = 1,
                .bid_depth = 0
            },
            inst_info_t {
                .name = "ret",
                .code = 0x3,
                .min_args = 0,
                .max_args = 1,
                .bid_depth = 0
            }
        });

        return result;
    }

    template<typename T, size_t TOffset>
    std::list<inst_info_t> generate_universal_size_isa_info() {
        std::list<inst_info_t> result;

        result.append_range(std::initializer_list {
            inst_info_t {
                .name = "mov" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x0,
                .min_args = 2,
                .max_args = 2,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "push" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x1,
                .min_args = 0,
                .max_args = 1,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "pop" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x2,
                .min_args = 0,
                .max_args = 1,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "not" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x3,
                .min_args = 1,
                .max_args = 2,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "and" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x4,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "or" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x5,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "xor" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x6,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "shl" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x7,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "shr" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x8,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            }
        });

        return result;
    }

    template<typename T, char TPostfix, size_t TOffset>
    std::list<inst_info_t> generate_universal_type_isa_info() {
        std::list<inst_info_t> result;

        result.append_range(std::initializer_list {
            inst_info_t {
                .name = "add" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x0,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "sub" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x1,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "mul" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x2,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "div" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x3,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "mod" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x4,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "inc" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x5,
                .min_args = 1,
                .max_args = 2,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "dec" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x6,
                .min_args = 1,
                .max_args = 2,
                .bid_depth = sizeof(T) * 8
            },
            inst_info_t {
                .name = "cmp" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x7,
                .min_args = 2,
                .max_args = 2,
                .bid_depth = sizeof(T) * 8
            }
        });

        return result;
    }
    template<typename T, char TPostfix, size_t TOffset>
    std::list<inst_info_t> generate_signed_type_isa_info() {
        std::list<inst_info_t> result;

        result.append_range(generate_universal_type_isa_info<T, TPostfix, TOffset>());
        result.emplace_back(inst_info_t {
            .name = "neg" + std::to_string(sizeof(T) * 8) + 'i',
            .code = TOffset + 0x8,
            .min_args = 1,
            .max_args = 2,
            .bid_depth = sizeof(T) * 8
        });

        return result;
    }

    std::list<inst_info_t> generate_isa_info() {
        std::list<inst_info_t> result;

        // [halt, fcall, ret]
        result.append_range(generate_system_isa_info());

        // [mov push pop not and or xor shl shr] for [8 16 32 64]
        result.append_range(generate_universal_size_isa_info<uint8_t, 0x100>());
        result.append_range(generate_universal_size_isa_info<uint16_t, 0x110>());
        result.append_range(generate_universal_size_isa_info<uint32_t, 0x120>());
        result.append_range(generate_universal_size_isa_info<uint64_t, 0x130>());

        // [add sub mul div mod inc dec cmp] for [8u 16u 32u 64u]
        result.append_range(generate_universal_type_isa_info<uint8_t, 'u', 0x140>());
        result.append_range(generate_universal_type_isa_info<uint16_t, 'u', 0x150>());
        result.append_range(generate_universal_type_isa_info<uint32_t, 'u', 0x160>());
        result.append_range(generate_universal_type_isa_info<uint64_t, 'u', 0x170>());

        // [add sub mul div mod inc dec cmp neg] for [8i 16i 32i 64i]
        result.append_range(generate_universal_type_isa_info<uint8_t, 'i', 0x180>());
        result.append_range(generate_universal_type_isa_info<uint16_t, 'i', 0x190>());
        result.append_range(generate_universal_type_isa_info<uint32_t, 'i', 0x1A0>());
        result.append_range(generate_universal_type_isa_info<uint64_t, 'i', 0x1B0>());

        return result;
    }
}
