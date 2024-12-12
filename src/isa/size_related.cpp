#include "size_related.hpp"

//std
#include <array>
#include <functional>
#include <string>

//alone
#include "error_codes.hpp"
#include "vm_context.hpp"

//alone::isa
#include "isa/universal.hpp"

namespace ph = std::placeholders;

namespace alone::isa {
    template<typename T>
    size_t inst_mov(const context_t& ctx, const lib::args_data_t& metadata) {
        size_t offset = lib::inst_code_size;
        T* a0;
        T a1;

        switch (metadata[0]) {
        case lib::argument_type::direct:
            a0 = ctx.get<T>(ctx.ipx() + offset);
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
        case lib::argument_type::direct:
            a1 = *ctx.get<T>(ctx.ipx() + offset);
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

        *a0 = a1;

        return offset;
    }

    template<typename T>
    size_t inst_push(const context_t& ctx, const lib::args_data_t& metadata) {
        size_t offset = lib::inst_code_size;
        bool is_extended = true;
        T a0;

        switch (metadata[0]) {
        case lib::argument_type::direct:
            a0 = *ctx.get<T>(ctx.ipx() + offset);
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
        case lib::argument_type::empty:
            is_extended = false;
            break;
        }

        if (is_extended)
            *ctx.get<T>(ctx.spx()) = a0;
        ctx.spx() += sizeof(T);

        return offset;
    }
    template<typename T>
    size_t inst_pop(const context_t& ctx, const lib::args_data_t& metadata) {
        size_t offset = lib::inst_code_size;
        T* a0;

        switch (metadata[0]) {
        case lib::argument_type::direct:
            a0 = ctx.get<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size;
            break;
        case lib::argument_type::immediate:
            a0 = ctx.get<T>(ctx.ipx() + offset);
            offset += sizeof(T);
            break;
        case lib::argument_type::indirect_with_displacement:
            a0 = ctx.get_with_displacement<T>(ctx.ipx() + offset);
            offset += lib::machine_word_size * 2;
            break;
        case lib::argument_type::empty:
            a0 = ctx.get<T>(ctx.asx());
            break;
        }

        *a0 = *ctx.get_with_displacement<T>(ctx.spx() - sizeof(T));
        ctx.spx() -= sizeof(T);

        return offset;
    }

    template<typename T, size_t Offset>
    std::list<lib::inst_t> generate_universal_size_related_isa() {
        std::list<lib::inst_t> result;

        result.append_range(std::initializer_list {
            lib::inst_t(
                std::string("mov") + std::to_string(sizeof(T) * 8),
                Offset + 0x00,
                3,
                sizeof(T) * 8,
                std::bind(inst_mov<T>, ph::_1, ph::_2)
            ),
            lib::inst_t(
                std::string("push") + std::to_string(sizeof(T) * 8),
                Offset + 0x01,
                3,
                sizeof(T) * 8,
                std::bind(inst_push<T>, ph::_1, ph::_2)
            ),
            lib::inst_t(
                std::string("pop") + std::to_string(sizeof(T) * 8),
                Offset + 0x02,
                3,
                sizeof(T) * 8,
                std::bind(inst_pop<T>, ph::_1, ph::_2)
            )
        });
        //result.emplace_back(SIZE_RELATED(""))

        return result;
    }

    std::list<lib::inst_t> generate_size_related_isa() {
        std::list<lib::inst_t> result;

        result.append_range(generate_universal_size_related_isa<uint8_t, 0x100>());
        result.append_range(generate_universal_size_related_isa<uint16_t, 0x110>());
        result.append_range(generate_universal_size_related_isa<uint32_t, 0x120>());
        result.append_range(generate_universal_size_related_isa<uint64_t, 0x130>());

        return result;
    }
}
