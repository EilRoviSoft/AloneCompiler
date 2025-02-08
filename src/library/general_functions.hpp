#pragma once

//std
#include <functional>
#include <string>

//frozen
#include "frozen/unordered_map.h"
#include "frozen/string.h"

//library
#include "library/types.hpp"

namespace amasm::lib::ext {
#include "library/crc64.hpp"
}

namespace amasm::lib {
    inline size_t hash_string(const std::string& what) {
        return ext::crc64(0, what.data(), what.size());
    }
    constexpr size_t hash_cstr(const char* what, size_t length) {
        return ext::crc64(0, what, length);
    }

    constexpr bool is_alpha(char c) {
        return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
    }
    constexpr bool is_binary(char c) {
        return c == '0' || c == '1';
    }
    constexpr bool is_numeric(char c) {
        return c >= '0' && c <= '9';
    }
    constexpr bool is_hexadecimal(char c) {
        return is_numeric(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
    }
    constexpr bool is_alpha_numeric(char c) {
        return is_alpha(c) || is_numeric(c);
    }
    constexpr bool is_whitespace(char c) {
        return c == ' ' || c == '\n' || c == '\t';
    }

    constexpr RegisterType cstr_to_register(const char* data, size_t size) {
        // 8 + 11 * 3 = 41
        constexpr frozen::unordered_map<frozen::string, RegisterType, 41> reference_dict = {
            { "al", AL }, { "ah", AH }, { "bl", BL }, { "bh", BH },
            { "cl", CL }, { "ch", CH }, { "dl", DL }, { "dh", DH },
            { "ax", AX }, { "bx", BX }, { "cx", CX }, { "dx", DX }, { "si", SI }, { "di", DI },
            { "ip", IP }, { "bp", BP }, { "sp", SP }, { "flags", FLAGS }, { "gp", GP },
            { "eax", EAX }, { "ebx", EBX }, { "ecx", ECX }, { "edx", EDX }, { "esi", ESI }, { "edi", EDI },
            { "eip", EIP }, { "ebp", EBP }, { "esp", ESP }, { "eflags", EFLAGS }, { "egp", EGP },
            { "rax", RAX }, { "rbx", RBX }, { "rcx", RCX }, { "rdx", RDX }, { "rsi", RSI }, { "rdi", RDI },
            { "rip", RIP }, { "rbp", RBP }, { "rsp", RSP }, { "rflags", RFLAGS }, { "rgp", RGP }
        };
        return reference_dict.at(frozen::string(data, size));
    }
    inline RegisterType str_to_register(const std::string& str) {
        return cstr_to_register(str.data(), str.size());
    }

    template<typename T, typename TBinaryOp, typename TRule>
    T for_each_by_rule(std::ranges::iterator_t<T> first,
                       std::ranges::iterator_t<T> last,
                       T init,
                       TBinaryOp bop,
                       TRule rule) {
        while (first != last) {
            T temp = rule(first);
            init = bop(std::move(init), temp);
        }
        return init;
    }

    template<typename T, std::ranges::input_range TRange, typename TBinaryOp, typename TRule>
    T for_each_by_rule(const TRange& range,
                       T init,
                       TBinaryOp bop,
                       TRule rule) {
        for (const auto& it : range) {
            T temp = rule(it);
            init = bop(std::move(init), temp);
        }
        return init;
    }

    template<typename TOut, typename TIn>
    TOut view_as(TIn* ptr) {
        return reinterpret_cast<TOut>(ptr);
    }

    template<typename T>
    struct bit_shl {
        constexpr auto operator()(T lhs, T rhs) const {
            return lhs << rhs;
        }
    };
    template<typename T>
    struct bit_shr {
        constexpr auto operator()(T lhs, T rhs) const {
            return lhs >> rhs;
        }
    };
}
