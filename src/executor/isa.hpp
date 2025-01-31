#pragma once

//std
#include <span>

//library
#include "library/types.hpp"

//executor
#include "executor/context.hpp"

namespace amasm::executor::isa {
    // system

    inline void halt(const Context& ctx, lib::args_data args) {
        ctx.flags()[RF] = false;
        ctx.reg(IPX) += lib::inst_size;
    }

    inline void fcall(const Context& ctx, lib::args_data args) {
        *ctx.get(ctx.reg(SPX)) = ctx.reg(BPX);
        ctx.reg(SPX) += lib::machine_word_size;

        ctx.reg(BPX) = ctx.reg(IPX) + lib::inst_size + lib::machine_word_size;
        ctx.reg(IPX) = *ctx.get(ctx.reg(IPX) + lib::inst_size);
    }
    inline void ret(const Context& ctx, lib::args_data args) {
        auto func_start = *ctx.get(ctx.reg(BPX) - lib::machine_word_size);
        auto args_size = *ctx.get(func_start - 16);
        auto ret_size = *ctx.get(func_start - 8);
        auto data = ctx.get<std::byte>(ctx.reg(IPX) + lib::inst_size);
        auto result = ctx.get<std::byte>(ret_size <= 8 ? AX : GPX);

        for (size_t i = 0; i < ret_size; i++)
            result[i] = data[i];

        ctx.reg(SPX) -= args_size + lib::machine_word_size;
        ctx.reg(IPX) = ctx.reg(BPX);
        ctx.reg(BPX) = *ctx.get(ctx.reg(SPX) + args_size);
    }

    // size related

    template<typename T>
    void inst_mov(const Context& ctx, lib::args_data args) {
        ptrdiff_t offset = lib::inst_size;
        T* a0;
        T a1;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[1]) {
        case ArgumentType::Direct:
            a1 = *ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a1 = *ctx.get<T>(ctx.reg(IPX) + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a1 = *ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        *a0 = a1;
        ctx.reg(IPX) += offset;
    }

    template<typename T>
    void inst_push(const Context& ctx, lib::args_data args) {
        ptrdiff_t offset = lib::inst_size;
        bool is_extended = true;
        T a0;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = *ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a0 = *ctx.get<T>(ctx.reg(IPX) + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = *ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        case ArgumentType::Empty:
            is_extended = false;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        if (is_extended)
            *ctx.get<T>(ctx.reg(SPX)) = a0;
        ctx.reg(SPX) += sizeof(T);
        ctx.reg(IPX) += offset;
    }
    template<typename T>
    void inst_pop(const Context& ctx, lib::args_data args) {
        ptrdiff_t offset = lib::inst_size;
        T* a0;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a0 = ctx.get<T>(ctx.reg(IPX) + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        case ArgumentType::Empty:
            a0 = reinterpret_cast<T*>(&ctx.reg(AX));
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        *a0 = *ctx.get_direct<T>(ctx.reg(SPX) - sizeof(T));
        ctx.reg(SPX) -= sizeof(T);
        ctx.reg(IPX) += offset;
    }

    // universal

    template<typename T, typename TPred>
    void inst_b23(const Context& ctx, lib::args_data args) {
        TPred pred;
        ptrdiff_t offset = lib::inst_size;
        bool is_extended = true;
        T* a0;
        T a1, a2;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[1]) {
        case ArgumentType::Direct:
            a1 = *ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a1 = *ctx.get<T>(ctx.reg(IPX) + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a1 = *ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[2]) {
        case ArgumentType::Direct:
            a2 = *ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a2 = *ctx.get<T>(ctx.reg(IPX) + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a2 = *ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        case ArgumentType::Empty:
            is_extended = false;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        *a0 = is_extended ? pred(a1, a2) : pred(*a0, a1);
        ctx.reg(IPX) += offset;
    }
    template<typename T, typename TPred>
    void inst_u12(const Context& ctx, lib::args_data args) {
        TPred pred;
        ptrdiff_t offset = lib::inst_size;
        bool is_extended = true;
        T* a0;
        T a1;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[1]) {
        case ArgumentType::Direct:
            a1 = *ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a1 = *ctx.get<T>(ctx.reg(IPX) + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a1 = *ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        case ArgumentType::Empty:
            is_extended = false;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        *a0 = is_extended ? pred(a1) : pred(*a0);
        ctx.reg(IPX) += offset;
    }

    template<typename T>
    void inst_cmp(const Context& ctx, lib::args_data args) {
        ptrdiff_t offset = lib::inst_size;
        T a0, a1;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = *ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a0 = *ctx.get<T>(ctx.reg(IPX) + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = *ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[1]) {
        case ArgumentType::Direct:
            a1 = *ctx.get_direct<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a1 = *ctx.get<T>(ctx.reg(IPX) + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a1 = *ctx.get_with_displacement<T>(ctx.reg(IPX) + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        ctx.flags()[ZF] = !(a0 - a1);
        ctx.flags()[SF] = (a0 - a1) >> (sizeof(T) * 8 - 1);
        ctx.reg(IPX) += offset;
    }
}
