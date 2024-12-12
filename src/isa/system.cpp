#include "system.hpp"

//alone
#include "instructions.hpp"
#include "vm_context.hpp"

namespace ph = std::placeholders;

namespace alone::isa {
    size_t halt(const context_t& ctx, const lib::args_data_t& metadata) {
        ctx.flags()[(size_t) lib::flags_set::cf] = false;
        return lib::inst_code_size;
    }

    size_t fcall(const context_t& ctx, const lib::args_data_t& metadata) {
        size_t offset = lib::inst_code_size;

        *ctx.get(ctx.spx()) = ctx.bpx(); //pushes value
        ctx.spx() += lib::machine_word_size;
        ctx.bpx() = ctx.ipx();
        ctx.ipx() = *ctx.get(ctx.ipx() + offset);
        offset += lib::machine_word_size;

        return offset;
    }
    size_t ret(const context_t& ctx, const lib::args_data_t& metadata) {
        size_t offset = lib::inst_code_size;
        bool is_extended = true;
        const auto &args_size = *ctx.get(ctx.bpx() - 16),
                   &ret_size = *ctx.get(ctx.bpx() - 8);
        auto a0 = std::span(ctx.mframe.begin() + ctx.ipx(), ctx.mframe.begin() + ctx.ipx() + ret_size);
        auto result_register = ret_size < 8 ? lib::regs_set::asx : lib::regs_set::grx;

        for (size_t i = 0; i != ret_size; i++)
            *ctx.get<std::byte>((size_t) result_register + i) = a0.data()[i];
        offset += ret_size;

        ctx.ipx() = ctx.bpx();
        ctx.spx() -= args_size + 8;
        ctx.bpx() = *ctx.get(ctx.spx());

        return offset;
    }

    //TODO: ncall, enter, leave
    std::list<lib::inst_t> generate_system_isa() {
        std::list<lib::inst_t> result;

        result.append_range(std::initializer_list {
            make_instruction(
                "halt",
                0x0,
                0,
                0,
                0,
                std::bind(halt, ph::_1, ph::_2)
            ),
            make_instruction(
                "fcall",
                0x2,
                1,
                1,
                0,
                std::bind(fcall, ph::_1, ph::_2)
            ),
            make_instruction(
                "ret",
                0x3,
                0,
                1,
                0,
                std::bind(ret, ph::_1, ph::_2)
            )
        });

        return result;
    }
}
