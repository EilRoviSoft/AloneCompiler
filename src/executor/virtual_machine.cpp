#include "virtual_machine.hpp"

//std
#include <functional>
#include <list>

//shared
#include "shared/general_functions.hpp"
#include "shared/types.hpp"

//executor
#include "executor/context.hpp"

//isa
#include "executor/isa.hpp"

namespace amasm::executor {
    using inst_pair = std::pair<shared::inst_code, inst_func>;

    // gisa stands for [g]enerate_[isa]

    std::list<inst_pair> gisa_system() {
        return {
            std::make_pair(0x0, isa::halt),
            std::make_pair(0x2, isa::fcall),
            std::make_pair(0x3, isa::ret),
        };
    }
    template<typename T, size_t Offset>
    std::list<inst_pair> gisa_generic_size_related() {
        return {
            std::make_pair(Offset + 0x0, isa::inst_mov<T>),
            std::make_pair(Offset + 0x1, isa::inst_push<T>),
            std::make_pair(Offset + 0x2, isa::inst_pop<T>),
            std::make_pair(Offset + 0x3, isa::inst_u12<T, std::bit_not<T>>),
            std::make_pair(Offset + 0x4, isa::inst_b23<T, std::bit_and<T>>),
            std::make_pair(Offset + 0x5, isa::inst_b23<T, std::bit_or<T>>),
            std::make_pair(Offset + 0x6, isa::inst_b23<T, std::bit_xor<T>>),
            std::make_pair(Offset + 0x7, isa::inst_b23<T, shared::bit_shl<T>>),
            std::make_pair(Offset + 0x8, isa::inst_b23<T, shared::bit_shr<T>>)
        };
    }
    std::list<inst_pair> gisa_size_related() {
        std::list<inst_pair> result;
        result.append_range(gisa_generic_size_related<uint8_t, 0x100>());
        result.append_range(gisa_generic_size_related<uint16_t, 0x110>());
        result.append_range(gisa_generic_size_related<uint32_t, 0x120>());
        result.append_range(gisa_generic_size_related<uint64_t, 0x130>());
        return result;
    }
    template<typename T, size_t Offset>
    std::list<inst_pair> gisa_generic_type_related() {
        return {
            std::make_pair(Offset + 0x0, isa::inst_b23<T, std::plus<T>>),
            std::make_pair(Offset + 0x1, isa::inst_b23<T, std::minus<T>>),
            std::make_pair(Offset + 0x2, isa::inst_b23<T, std::multiplies<T>>),
            std::make_pair(Offset + 0x3, isa::inst_b23<T, std::divides<T>>),
            std::make_pair(Offset + 0x4, isa::inst_b23<T, std::modulus<T>>),
            std::make_pair(Offset + 0x5, isa::inst_cmp<T>),
        };
    }
    template<typename T, size_t Offset>
    std::list<inst_pair> gisa_signed_type_related() {
        std::list<inst_pair> result;
        result.append_range(gisa_generic_type_related<T, Offset>());
        result.emplace_back(Offset + 0x6, isa::inst_u12<T, std::negate<T>>);
        return result;
    }
    std::list<inst_pair> gisa_type_related() {
        std::list<inst_pair> result;
        result.append_range(gisa_generic_type_related<uint8_t, 0x140>());
        result.append_range(gisa_generic_type_related<uint16_t, 0x150>());
        result.append_range(gisa_generic_type_related<uint32_t, 0x160>());
        result.append_range(gisa_generic_type_related<uint64_t, 0x170>());
        result.append_range(gisa_signed_type_related<int8_t, 0x180>());
        result.append_range(gisa_signed_type_related<int16_t, 0x190>());
        result.append_range(gisa_signed_type_related<int32_t, 0x1A0>());
        result.append_range(gisa_signed_type_related<int64_t, 0x1B0>());
        return result;
    }

    void VirtualMachine::init_instructions() {
        if (!_instructions.empty())
            _instructions.clear();

        for (const auto& it : gisa_system())
            _instructions.emplace(it);
        for (const auto& it : gisa_size_related())
            _instructions.emplace(it);
        for (const auto& it : gisa_type_related())
            _instructions.emplace(it);
    }

    void VirtualMachine::exec(Context& ctx, shared::Bytecode program) {
        ctx.set_parent(*this);

        ctx._program_size = program.size();
        for (size_t i = shared::machine_word_size; i < ctx.program_size(); i++)
            _mframe[i + shared::registers_size] = program[i];

        const size_t stack_start = shared::registers_size + ctx._program_size;
        ctx.flags()[RF] = true;
        ctx.reg(SPX) = stack_start;
        ctx.reg(IPX) = *reinterpret_cast<const shared::machine_word*>(program.data()) + shared::registers_size;

        while (ctx.flags()[RF] && ctx.reg(IPX) < stack_start) {
            const auto& inst = *ctx.get<shared::inst_code>(ctx.reg(IPX));
            auto [opcode, args_metadata] = decompose_instruction(inst);
            const size_t offset = _instructions.at(opcode)(ctx, args_metadata);
            ctx.reg(IPX) += offset;
        }
    }
}
