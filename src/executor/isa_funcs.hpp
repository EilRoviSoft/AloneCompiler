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
        ctx[RF] = false;
        ctx[IP] += lib::inst_size;
    }

    inline void fcall(const Context& ctx, lib::args_data args) {
        *ctx.get(ctx[SP]) = ctx[BP];
        ctx[SP] += lib::machine_word_size;

        ctx[BP] = ctx[IP] + lib::inst_size + lib::machine_word_size;
        ctx[IP] = *ctx.get(ctx[IP] + lib::inst_size);
    }
    inline void ret(const Context& ctx, lib::args_data args) {
        auto func_start = *ctx.get(ctx[BP] - lib::machine_word_size);
        auto args_size = *ctx.get(func_start - 16);
        auto ret_size = *ctx.get(func_start - 8);
        auto data = ctx.get<std::byte>(ctx[IP] + lib::inst_size);
        auto result = ctx.get<std::byte>(ret_size <= 8 ? AX : GP);

        for (size_t i = 0; i < ret_size; i++)
            result[i] = data[i];

        ctx[SP] -= args_size + lib::machine_word_size;
        ctx[IP] = ctx[BP];
        ctx[BP] = *ctx.get(ctx[SP] + args_size);
    }

    // size related

    template<typename T>
    void inst_mov(const Context& ctx, lib::args_data args) {
        ptrdiff_t offset = lib::inst_size;
        T* a0;
        T a1;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[1]) {
        case ArgumentType::Direct:
            a1 = *ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a1 = *ctx.get<T>(ctx[IP] + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a1 = *ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        *a0 = a1;
        ctx[IP] += offset;
    }

    template<typename T>
    void inst_push(const Context& ctx, lib::args_data args) {
        ptrdiff_t offset = lib::inst_size;
        bool is_extended = true;
        T a0;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = *ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a0 = *ctx.get<T>(ctx[IP] + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = *ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        case ArgumentType::Empty:
            is_extended = false;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        if (is_extended)
            *ctx.get<T>(ctx[SP]) = a0;
        ctx[SP] += sizeof(T);
        ctx[IP] += offset;
    }
    template<typename T>
    void inst_pop(const Context& ctx, lib::args_data args) {
        ptrdiff_t offset = lib::inst_size;
        T* a0;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a0 = ctx.get<T>(ctx[IP] + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        case ArgumentType::Empty:
            a0 = reinterpret_cast<T*>(&ctx[AX]);
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        *a0 = *ctx.get_direct<T>(ctx[SP] - sizeof(T));
        ctx[SP] -= sizeof(T);
        ctx[IP] += offset;
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
            a0 = ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[1]) {
        case ArgumentType::Direct:
            a1 = *ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a1 = *ctx.get<T>(ctx[IP] + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a1 = *ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[2]) {
        case ArgumentType::Direct:
            a2 = *ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a2 = *ctx.get<T>(ctx[IP] + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a2 = *ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        case ArgumentType::Empty:
            is_extended = false;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        *a0 = is_extended ? pred(a1, a2) : pred(*a0, a1);
        ctx[IP] += offset;
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
            a0 = ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[1]) {
        case ArgumentType::Direct:
            a1 = *ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a1 = *ctx.get<T>(ctx[IP] + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a1 = *ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        case ArgumentType::Empty:
            is_extended = false;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        *a0 = is_extended ? pred(a1) : pred(*a0);
        ctx[IP] += offset;
    }

    template<typename T>
    void inst_cmp(const Context& ctx, lib::args_data args) {
        ptrdiff_t offset = lib::inst_size;
        T a0, a1;

        switch (args[0]) {
        case ArgumentType::Direct:
            a0 = *ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a0 = *ctx.get<T>(ctx[IP] + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a0 = *ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        switch (args[1]) {
        case ArgumentType::Direct:
            a1 = *ctx.get_direct<T>(ctx[IP] + offset);
            offset += lib::machine_word_size;
            break;
        case ArgumentType::Immediate:
            a1 = *ctx.get<T>(ctx[IP] + offset);
            offset += sizeof(T);
            break;
        case ArgumentType::IndirectWithDisplacement:
            a1 = *ctx.get_with_displacement<T>(ctx[IP] + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw std::runtime_error("wrong args information");
        }

        ctx[ZF] = !(a0 - a1);
        ctx[SF] = (a0 - a1) >> (sizeof(T) * 8 - 1);
        ctx[IP] += offset;
    }
}
