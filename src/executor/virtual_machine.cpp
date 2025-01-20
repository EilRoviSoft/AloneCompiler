#include "virtual_machine.hpp"

//std
#include <functional>
#include <list>

//shared
#include "shared/general_functions.hpp"
#include "shared/types.hpp"

//executor
#include "executor/constants.hpp"
#include "executor/context.hpp"

//isa
#include "executor/isa.hpp"

namespace amasm::executor {
    // gisa stands for [g]enerate_[isa]

    std::list<InstructionPair> gisa_system() {
        return {
            make_instruction(0x0, isa::halt),
            make_instruction(0x2, isa::fcall, 8),
            make_instruction(0x3, isa::ret, -8),
        };
    }
    template<typename T, size_t Offset>
    std::list<InstructionPair> gisa_generic_size_related() {
        return {
            make_instruction(Offset + 0x0, isa::inst_mov<T>),
            make_instruction(Offset + 0x1, isa::inst_push<T>, sizeof(T)),
            make_instruction(Offset + 0x2, isa::inst_pop<T>),
            make_instruction(Offset + 0x3, isa::inst_u12<T, std::bit_not<T>>),
            make_instruction(Offset + 0x4, isa::inst_b23<T, std::bit_and<T>>),
            make_instruction(Offset + 0x5, isa::inst_b23<T, std::bit_or<T>>),
            make_instruction(Offset + 0x6, isa::inst_b23<T, std::bit_xor<T>>),
            make_instruction(Offset + 0x7, isa::inst_b23<T, shared::bit_shl<T>>),
            make_instruction(Offset + 0x8, isa::inst_b23<T, shared::bit_shr<T>>)
        };
    }
    std::list<InstructionPair> gisa_size_related() {
        std::list<InstructionPair> result;
        result.append_range(gisa_generic_size_related<uint8_t, 0x100>());
        result.append_range(gisa_generic_size_related<uint16_t, 0x110>());
        result.append_range(gisa_generic_size_related<uint32_t, 0x120>());
        result.append_range(gisa_generic_size_related<uint64_t, 0x130>());
        return result;
    }
    template<typename T, size_t Offset>
    std::list<InstructionPair> gisa_generic_type_related() {
        return {
            make_instruction(Offset + 0x0, isa::inst_b23<T, std::plus<T>>),
            make_instruction(Offset + 0x1, isa::inst_b23<T, std::minus<T>>),
            make_instruction(Offset + 0x2, isa::inst_b23<T, std::multiplies<T>>),
            make_instruction(Offset + 0x3, isa::inst_b23<T, std::divides<T>>),
            make_instruction(Offset + 0x4, isa::inst_b23<T, std::modulus<T>>),
            make_instruction(Offset + 0x5, isa::inst_cmp<T>),
        };
    }
    template<typename T, size_t Offset>
    std::list<InstructionPair> gisa_signed_type_related() {
        std::list<InstructionPair> result;
        result.append_range(gisa_generic_type_related<T, Offset>());
        result.emplace_back(make_instruction(Offset + 0x6, isa::inst_u12<T, std::negate<T>>));
        return result;
    }
    std::list<InstructionPair> gisa_type_related() {
        std::list<InstructionPair> result;
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

    void VirtualMachine::init() {
        _init(mframe_size, mframe_size);
    }
    void VirtualMachine::init(shared::machine_word size) {
        _init(size, size);
    }
    void VirtualMachine::init(shared::machine_word min_size, shared::machine_word max_size) {
        _init(min_size, max_size);
    }

    void VirtualMachine::exec(Context& ctx, shared::Bytecode program) {
        ctx.set_parent(*this);

        ctx._program_size = program.size();
        for (size_t i = shared::machine_word_size; i < ctx.program_size(); i++)
            _mframe[i + shared::registers_size] = program[i];

        //_stack_start = shared::registers_size + ctx._program_size;
        ctx.flags()[RF] = true;
        ctx.reg(SPX) = ctx._program_size + shared::registers_size;
        ctx.reg(IPX) = *reinterpret_cast<const shared::machine_word*>(program.data()) + shared::registers_size;

        const StepperSignature stepper = _min_size == _max_size
            ? &VirtualMachine::_do_step
            : &VirtualMachine::_do_step_with_check;

        while (ctx.flags()[RF] && ctx.reg(IPX) < ctx._program_size + shared::registers_size)
            stepper(this, ctx);
    }

    void VirtualMachine::_init(shared::machine_word min_size, shared::machine_word max_size) {
        _init_memory(min_size, max_size);
        _init_instructions();
    }
    void VirtualMachine::_init_memory(shared::machine_word min_size, shared::machine_word max_size) {
        _min_size = min_size;
        _max_size = max_size;
        _mframe.resize(min_size);
    }
    void VirtualMachine::_init_instructions() {
        if (!_instructions.empty())
            _instructions.clear();

        for (const auto& it : gisa_system())
            _instructions.emplace(it);
        for (const auto& it : gisa_size_related())
            _instructions.emplace(it);
        for (const auto& it : gisa_type_related())
            _instructions.emplace(it);
    }

    void VirtualMachine::_do_step(const Context& ctx) {
        const auto inst_code = *ctx.get<shared::inst_code>(ctx.reg(IPX));
        const auto [opcode, args_metadata] = decompose_instruction(inst_code);
        const auto& inst = _instructions.at(opcode);
        inst(ctx, args_metadata);
    }
    void VirtualMachine::_do_step_with_check(const Context& ctx) {
        const auto inst_code = *ctx.get<shared::inst_code>(ctx.reg(IPX));
        const auto [opcode, args_metadata] = decompose_instruction(inst_code);
        const auto& inst = _instructions.at(opcode);

        if (ctx.reg(SPX) + inst._memory_shift < _mframe.size())
            _alloc_more_memory();

        inst(ctx, args_metadata);
    }

    size_t VirtualMachine::_alloc_more_memory() {
        size_t old_size = _mframe.size();
        size_t new_size = old_size * 2 < _max_size ? old_size * 2 : _max_size;
        _mframe.resize(new_size);
        return new_size - old_size;
    }
}
