#include "system.hpp"

//alone
#include "instructions.hpp"
#include "vm_context.hpp"

namespace ph = std::placeholders;

namespace alone::isa {
    size_t halt(const context_t& ctx, const lib::args_data_t& metadata) {
        ctx.flags()[(size_t) lib::flags_set::rf] = false;
        return lib::inst_code_size;
    }

    size_t fcall(const context_t& ctx, const lib::args_data_t& metadata) {
        *ctx.get(ctx.spx()) = ctx.bpx();                          //push64 %bpx
        *ctx.get(ctx.spx() + lib::machine_word_size) = ctx.cpx(); //push64 %cpx
        ctx.spx() += lib::machine_word_size * 2;                  //add64 %spx, reg_size * 2
        
        ctx.bpx() = ctx.ipx() + lib::inst_code_size + lib::machine_word_size;
        ctx.ipx() = *ctx.get(ctx.ipx() + lib::inst_code_size);
        ctx.cpx() = ctx.ipx();
        return 0;
    }
    size_t ret(const context_t& ctx, const lib::args_data_t& metadata) {
        const auto& args_size = *ctx.get(ctx.cpx() - 16);
        const auto& ret_size = *ctx.get(ctx.cpx() - 8);
        const auto actual_result_size = metadata[0] != lib::argument_type::empty ? ret_size : 0;
        const auto a0 = std::span(ctx.mframe.begin() + ctx.ipx() + lib::inst_code_size,
                                  ctx.mframe.begin() + ctx.ipx() + lib::inst_code_size + actual_result_size);
        auto result_register = ret_size <= 8 ? lib::regs_set::asx : lib::regs_set::grx;

        for (size_t i = 0; i < actual_result_size; i++)
            *ctx.get<std::byte>((size_t) result_register + i) = a0.data()[i];

        ctx.spx() -= args_size + lib::machine_word_size * 2;
        ctx.ipx() = ctx.bpx();

        ctx.bpx() = *ctx.get(ctx.spx() + args_size);
        ctx.cpx() = *ctx.get(ctx.spx() + args_size + lib::machine_word_size);

        return 0;
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
