#pragma once

//std
#include <array>
#include <functional>

//alone
#include "error_codes.hpp"
#include "instructions.hpp"
#include "vm_context.hpp"

namespace alone::isa {
    template<typename T>
    size_t inst_b23(const context_t& ctx,
                    const lib::args_data_t& metadata,
                    const std::function<T(const T&, const T&)>& pred) {
        size_t offset = lib::inst_code_size;
        bool is_extended = true;
        T* a0;
        T a1, a2;

        switch (metadata[0]) {
        case lib::argument_type::indirect:
            a0 = ctx.get_indirect<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size;
            break;
        case lib::argument_type::indirect_with_displacement:
            a0 = ctx.get_with_displacement<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw ISA_WRONG_ARGS_METADATA;
        }

        switch (metadata[1]) {
        case lib::argument_type::indirect:
            a1 = *ctx.get_indirect<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size;
            break;
        case lib::argument_type::immediate:
            a1 = *ctx.get<T>(ctx.ipx() + offset);
            offset += sizeof(T);
            break;
        case lib::argument_type::indirect_with_displacement:
            a1 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw ISA_WRONG_ARGS_METADATA;
        }

        switch (metadata[2]) {
        case lib::argument_type::indirect:
            a2 = *ctx.get_indirect<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size;
            break;
        case lib::argument_type::immediate:
            a2 = *ctx.get<T>(ctx.ipx() + offset);
            offset += sizeof(T);
            break;
        case lib::argument_type::indirect_with_displacement:
            a2 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size * 2;
            break;
        case lib::argument_type::empty:
            is_extended = false;
            break;
        }

        *a0 = is_extended ? pred(a1, a2) : pred(*a0, a1);

        return offset;
    }

    template<typename T>
    size_t inst_u12(const context_t& ctx, const lib::args_data_t& metadata, const std::function<T(const T&)>& pred) {
        size_t offset = lib::inst_code_size;
        bool is_extended = true;
        T* a0;
        T a1;

        switch (metadata[0]) {
        case lib::argument_type::indirect:
            a0 = ctx.get_indirect<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size;
            break;
        case lib::argument_type::indirect_with_displacement:
            a0 = ctx.get_with_displacement<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw ISA_WRONG_ARGS_METADATA;
        }

        switch (metadata[1]) {
        case lib::argument_type::indirect:
            a1 = *ctx.get_indirect<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size;
            break;
        case lib::argument_type::immediate:
            a1 = *ctx.get<T>(ctx.ipx() + offset);
            offset += sizeof(T);
            break;
        case lib::argument_type::indirect_with_displacement:
            a1 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size * 2;
            break;
        case lib::argument_type::empty:
            is_extended = false;
            break;
        }

        *a0 = is_extended ? pred(a1) : pred(*a0);

        return offset;
    }

    template<typename T>
    size_t inst_cmp(const context_t& ctx, const lib::args_data_t& metadata) {
        size_t offset = lib::inst_code_size;
        T a0, a1;

        switch (metadata[0]) {
        case lib::argument_type::indirect:
            a0 = *ctx.get_indirect<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size;
            break;
        case lib::argument_type::immediate:
            a0 = *ctx.get<T>(ctx.ipx() + offset);
            offset += sizeof(T);
            break;
        case lib::argument_type::indirect_with_displacement:
            a0 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw ISA_WRONG_ARGS_METADATA;
        }

        switch (metadata[1]) {
        case lib::argument_type::indirect:
            a1 = *ctx.get_indirect<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size;
            break;
        case lib::argument_type::immediate:
            a1 = *ctx.get<T>(ctx.ipx() + offset);
            offset += sizeof(T);
            break;
        case lib::argument_type::indirect_with_displacement:
            a1 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size * 2;
            break;
        default:
            throw ISA_WRONG_ARGS_METADATA;
        }

        ctx.flags()[(size_t) lib::flags_set::zf] = !(a0 - a1);
        ctx.flags()[(size_t) lib::flags_set::sf] = (a0 - a1) >> (sizeof(T) * 8 - 1);

        return offset;
    }
}
