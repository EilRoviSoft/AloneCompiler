#include "util.hpp"

namespace amasm::executor {
    std::tuple<lib::address, MemoryType> decompose_address(lib::address address) {
        return {
            (address & 0x3fffffffffffffff),
            MemoryType((address & 0xC000000000000000) >> (sizeof(lib::address) * 8 - 2))
        };
    }
    std::tuple<lib::inst_code, lib::args_data> decompose_instruction(lib::inst_code inst) {
        const size_t args_metadata = (inst & 0xFF000000) >> 24;
        return std::make_tuple(
            inst & 0x00FFFFFF,
            lib::args_data {
                ArgumentType((args_metadata & 0b00000011) >> 0),
                ArgumentType((args_metadata & 0b00001100) >> 2),
                ArgumentType((args_metadata & 0b00110000) >> 4),
                ArgumentType((args_metadata & 0b11000000) >> 6)
            }
        );
    }
}
