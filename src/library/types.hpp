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
    enum class AddressType : uint8_t {
        Empty = 0,        // none
        Relative,         // idk how to define it now
        RelativeWithDiff, // name + value
        Fixed             // value
    };

    enum RegisterType : uint8_t {
        // 16-bit registers

        AX = SYSTEM_MW_SIZE * 0, BX = SYSTEM_MW_SIZE * 1,
        CX = SYSTEM_MW_SIZE * 2, DX = SYSTEM_MW_SIZE * 3,
        SI = SYSTEM_MW_SIZE * 4, DI = SYSTEM_MW_SIZE * 5,
        IP = SYSTEM_MW_SIZE * 6, BP = SYSTEM_MW_SIZE * 7, SP = SYSTEM_MW_SIZE * 8,
        GP = SYSTEM_MW_SIZE * 10,

        // 32-bit registers

        EAX = AX, EBX = BX,
        ECX = CX, EDX = DX,
        ESI = SI, EDI = DI,
        EIP = IP, EBP = BP, ESP = SP,
        EGP = GP,

        // 64-bit registers

        RAX = AX, RBX = BX,
        RCX = CX, RDX = DX,
        RSI = SI, RDI = DI,
        RIP = IP, RBP = BP, RSP = SP,
        RGP = GP,

        // FLAGS registers

        FLAGS = SYSTEM_MW_SIZE * 9, EFLAGS = FLAGS, RFLAGS = FLAGS,
    };

    enum FlagType : uint8_t {
        RF = 0x00, // run
        CF = 0x01, // carry
        PF = 0x02, // parity
        ZF = 0x03, // zero
        SF = 0x04, // sign
        IF = 0x05, // interrupt enable
        DF = 0x06, // direction
        OF = 0x07, // overflow
    };
}

namespace amasm::lib {
    using byte_vector = std::vector<SYSTEM_MIN_TYPE>;
    using machine_word = SYSTEM_MW_TYPE;
    using address = SYSTEM_MW_TYPE;
    using inst_code = SYSTEM_INST_TYPE;
    using flags = std::bitset<SYSTEM_BIT_DEPTH>;
    using args_data = std::array<AddressType, 4>;

    constexpr size_t machine_word_size = sizeof(SYSTEM_MW_TYPE);
    constexpr size_t address_size = sizeof(SYSTEM_MW_TYPE);
    constexpr size_t inst_size = sizeof(SYSTEM_INST_TYPE);

    constexpr size_t registers_size = 256;
    constexpr size_t system_registers_size = registers_size - GP;
    constexpr size_t general_registers_size = registers_size - system_registers_size;
}
