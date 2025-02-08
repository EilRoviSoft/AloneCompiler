#include "virtual_machine.hpp"

//std
#include <functional>

//library
#include "library/general_functions.hpp"
#include "library/types.hpp"

//executor
#include "executor/constants.hpp"
#include "executor/context.hpp"
#include "executor/isa_factory.hpp"

namespace amasm::executor {
    void VirtualMachine::init() {
        _init(mframe_size, mframe_size);
    }
    void VirtualMachine::init(lib::machine_word size) {
        _init(size, size);
    }
    void VirtualMachine::init(lib::machine_word min_size, lib::machine_word max_size) {
        _init(min_size, max_size);
    }

    void VirtualMachine::exec(lib::Bytecode program) {
        Context ctx(*this);

        ctx._program_size = program.size();
        for (size_t i = lib::machine_word_size; i < ctx.program_size(); i++)
            _mframe[i + lib::registers_size] = program[i];

        ctx[RF] = true;
        ctx[SP] = ctx._program_size + lib::registers_size;
        ctx[IP] = *reinterpret_cast<const lib::machine_word*>(program.data()) + lib::registers_size;

        const StepperSignature stepper = _min_size == _max_size
            ? &VirtualMachine::_do_step
            : &VirtualMachine::_do_step_with_check;

        while (ctx[RF] && ctx[IP] < ctx._program_size + lib::registers_size)
            stepper(this, ctx);
    }

    void VirtualMachine::_init(lib::machine_word min_size, lib::machine_word max_size) {
        _init_memory(min_size, max_size);
        _init_instructions();
    }
    void VirtualMachine::_init_memory(lib::machine_word min_size, lib::machine_word max_size) {
        _min_size = min_size;
        _max_size = max_size;
        _mframe.resize(min_size);
    }
    void VirtualMachine::_init_instructions() {
        if (!_instructions.empty())
            _instructions.clear();
        auto data = IsaFactory().generate_isa().get_product();

        while (!data.empty()) {
            auto on_emplace = std::move(data.back());
            data.pop_back();
            size_t key = on_emplace.id();
            _instructions.emplace(key, std::move(on_emplace));
        }

        for (auto&& it : data)
            _instructions.emplace(it.id(), it);
    }

    void VirtualMachine::_do_step(const Context& ctx) {
        const auto inst_code = *ctx.get<lib::inst_code>(ctx[IP]);
        const auto [opcode, args_metadata] = decompose_instruction(inst_code);
        const auto& inst = _instructions.at(opcode);
        inst(ctx, args_metadata);
    }
    void VirtualMachine::_do_step_with_check(const Context& ctx) {
        const auto inst_code = *ctx.get<lib::inst_code>(ctx[IP]);
        const auto [opcode, args_metadata] = decompose_instruction(inst_code);
        const auto& inst = _instructions.at(opcode);

        if (ctx[SP] + inst.m_memory_shift < _mframe.size())
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
