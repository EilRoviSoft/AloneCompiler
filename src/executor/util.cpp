#include "util.hpp"

namespace amasm::executor {
    std::tuple<shared::Address, MemoryType> decompose_address(shared::Address address) {
        return {
            (address & 0x3fffffffffffffff),
            MemoryType((address & 0xC000000000000000) >> (sizeof(shared::Address) * 8 - 2))
        };
    }
    std::tuple<shared::InstCode, shared::ArgumentsData> decompose_instruction(shared::InstCode inst) {
        const size_t args_metadata = (inst & 0xFF000000) >> 24;
        return std::make_tuple(
            inst & 0x00FFFFFF,
            shared::ArgumentsData {
                ArgumentType((args_metadata & 0b00000011) >> 0),
                ArgumentType((args_metadata & 0b00001100) >> 2),
                ArgumentType((args_metadata & 0b00110000) >> 4),
                ArgumentType((args_metadata & 0b11000000) >> 6)
            }
        );
    }
}
