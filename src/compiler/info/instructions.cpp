#include "instructions.hpp"

//std
#ifdef DEBUG_STATUS
#include <algorithm>
#include <iostream>
#endif

namespace amasm::compiler {
    std::list<inst_info> generate_system_isa_info() {
        std::list<inst_info> result;

        result.append_range(std::initializer_list {
            inst_info {
                .name = "halt",
                .code = 0x0,
                .min_args = 0,
                .max_args = 0,
                .bid_depth = 0
            },
            inst_info {
                .name = "fcall",
                .code = 0x2,
                .min_args = 1,
                .max_args = 1,
                .bid_depth = 0
            },
            inst_info {
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
    std::list<inst_info> generate_universal_size_isa_info() {
        std::list<inst_info> result;

        result.append_range(std::initializer_list {
            inst_info {
                .name = "mov" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x0,
                .min_args = 2,
                .max_args = 2,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "push" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x1,
                .min_args = 0,
                .max_args = 1,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "pop" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x2,
                .min_args = 0,
                .max_args = 1,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "not" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x3,
                .min_args = 1,
                .max_args = 2,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "and" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x4,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "or" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x5,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "xor" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x6,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "shl" + std::to_string(sizeof(T) * 8),
                .code = TOffset + 0x7,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
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
    std::list<inst_info> generate_universal_type_isa_info() {
        std::list<inst_info> result;

        result.append_range(std::initializer_list {
            inst_info {
                .name = "add" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x0,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "sub" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x1,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "mul" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x2,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "div" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x3,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "mod" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x4,
                .min_args = 2,
                .max_args = 3,
                .bid_depth = sizeof(T) * 8
            },
            inst_info {
                .name = "cmp" + std::to_string(sizeof(T) * 8) + TPostfix,
                .code = TOffset + 0x5,
                .min_args = 2,
                .max_args = 2,
                .bid_depth = sizeof(T) * 8
            }
        });

        return result;
    }
    template<typename T, char TPostfix, size_t TOffset>
    std::list<inst_info> generate_signed_type_isa_info() {
        std::list<inst_info> result;

        result.append_range(generate_universal_type_isa_info<T, TPostfix, TOffset>());
        result.emplace_back(inst_info {
            .name = "neg" + std::to_string(sizeof(T) * 8) + 'i',
            .code = TOffset + 0x8,
            .min_args = 1,
            .max_args = 2,
            .bid_depth = sizeof(T) * 8
        });

        return result;
    }

    std::list<inst_info> generate_isa_info() {
        std::list<inst_info> result;

        // [halt, fcall, ret]
        result.append_range(generate_system_isa_info());

        // [mov push pop not and or xor shl shr] for [8 16 32 64]
        result.append_range(generate_universal_size_isa_info<uint8_t, 0x100>());
        result.append_range(generate_universal_size_isa_info<uint16_t, 0x110>());
        result.append_range(generate_universal_size_isa_info<uint32_t, 0x120>());
        result.append_range(generate_universal_size_isa_info<uint64_t, 0x130>());

        // [add sub mul div mod cmp] for [8u 16u 32u 64u]
        result.append_range(generate_universal_type_isa_info<uint8_t, 'u', 0x140>());
        result.append_range(generate_universal_type_isa_info<uint16_t, 'u', 0x150>());
        result.append_range(generate_universal_type_isa_info<uint32_t, 'u', 0x160>());
        result.append_range(generate_universal_type_isa_info<uint64_t, 'u', 0x170>());

        // [add sub mul div mod cmp neg] for [8i 16i 32i 64i]
        result.append_range(generate_universal_type_isa_info<uint8_t, 'i', 0x180>());
        result.append_range(generate_universal_type_isa_info<uint16_t, 'i', 0x190>());
        result.append_range(generate_universal_type_isa_info<uint32_t, 'i', 0x1A0>());
        result.append_range(generate_universal_type_isa_info<uint64_t, 'i', 0x1B0>());

#ifdef DEBUG_STATUS
        const auto amount = std::ranges::count_if(result, [](const auto& it) { return it.name == "push64"; });
        //std::cerr << amount << '\n';
#endif

        return result;
    }
}
