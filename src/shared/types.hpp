#pragma once

//std
#include <array>
#include <bitset>
#include <cstddef>
#include <vector>

#define SYSTEM_MIN_TYPE std::byte
#define SYSTEM_MW_TYPE uint64_t
#define SYSTEM_INST_TYPE uint32_t
#define SYSTEM_MW_SIZE sizeof(SYSTEM_MW_TYPE)
#define SYSTEM_BIT_DEPTH 64

namespace amasm {
    enum class ArgumentType : uint8_t {
        Empty = 0,                // none
        Direct,                   // idk how to define it now
        IndirectWithDisplacement, // name + value
        Immediate,                // value
        JumpAddress               // value
    };

    enum RegisterType : uint8_t {
        // 8-bit registers

        AL = SYSTEM_MW_SIZE * 0,
        AH = AL + 1,
        BL = SYSTEM_MW_SIZE * 1,
        BH = BL + 1,
        CL = SYSTEM_MW_SIZE * 2,
        CH = CL + 1,
        DL = SYSTEM_MW_SIZE * 3,
        DH = DL + 1,

        // 16-bit registers

        AX    = AL,
        BX    = BL,
        CX    = CL,
        DX    = DL,
        IPX   = SYSTEM_MW_SIZE * 4,
        BPX   = SYSTEM_MW_SIZE * 5,
        CPX   = SYSTEM_MW_SIZE * 6,
        SPX   = SYSTEM_MW_SIZE * 7,
        FLAGS = SYSTEM_MW_SIZE * 8,
        GPX   = SYSTEM_MW_SIZE * 10,

        // 32-bit registers

        EAX    = AL,
        EBX    = BL,
        ECX    = CL,
        EDX    = DL,
        EIP    = IPX,
        EBP    = BPX,
        ECP    = CPX,
        ESP    = SPX,
        EFLAGS = FLAGS,
        EGP    = GPX,

        // 64-bit registers

        RAX    = AL,
        RBX    = BL,
        RCX    = CL,
        RDX    = DL,
        RIP    = IPX,
        RBP    = BPX,
        RCP    = CPX,
        RSP    = SPX,
        RFLAGS = FLAGS,
        RGP    = GPX
    };

    enum FlagType : uint8_t {
        RF = 0,
        ZF = 1,
        SF = 2,
        CF = 3,
        OF = 4
    };
}

namespace amasm::shared {
    using byte_vector = std::vector<SYSTEM_MIN_TYPE>;
    using machine_word = SYSTEM_MW_TYPE;
    using address = SYSTEM_MW_TYPE;
    using inst_code = SYSTEM_INST_TYPE;
    using flags = std::bitset<SYSTEM_BIT_DEPTH>;
    using args_data = std::array<ArgumentType, 4>;

    constexpr size_t machine_word_size = sizeof(SYSTEM_MW_TYPE);
    constexpr size_t address_size = sizeof(SYSTEM_MW_TYPE);
    constexpr size_t inst_size = sizeof(SYSTEM_INST_TYPE);

    constexpr size_t registers_size = 256;
    constexpr size_t system_registers_size = registers_size - GPX;
    constexpr size_t general_registers_size = registers_size - system_registers_size;
}
