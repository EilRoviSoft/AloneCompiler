#include "translator.hpp"

//std
#ifdef DEBUG_STATUS
#include <iostream>
#endif
#include <list>
#include <numeric>
#include <tuple>
#include <unordered_map>

//shared
#include "shared/general_functions.hpp"
#include "shared/types.hpp"

namespace amasm::compiler {
    Translator::Translator(Context& ctx) :
        _ctx(ctx) {
    }

    shared::Bytecode Translator::translate(funcs_queue composed) const {
        shared::Bytecode result;
        std::unordered_map<std::string, size_t> labels;
        std::list<hint> hints;

        // adds total size of program to the front of the code
        result.append_value<shared::machine_word>(0);
        while (!composed.empty()) {
            const auto func = std::move(composed.front());
            composed.pop();

            result.append_value<shared::machine_word>(shared::for_each_by_rule(
                func.args,
                0,
                std::plus {},
                [](const datatype_ptr& ptr) { return ptr->size; }
            ));
            result.append_value<shared::machine_word>(func.return_type->size);
            labels.emplace(generate_func_full_name(func), result.size());

            for (const auto& inst : func.lines) {
                auto [new_bytecode, new_hints] = _decompose_instruction(func, inst);
                for (auto& it : new_hints)
                    it.offset += result.size();
                hints.append_range(new_hints);
                result.append_sequence(new_bytecode);
            }
        }

        result.set(labels.at("@main()"), 0);
        for (const auto& it : hints)
            result.set(labels[it.name] + shared::registers_size, it.offset);

        return result;
    }

    std::tuple<shared::Bytecode, std::list<Translator::hint>> Translator::_decompose_instruction(
        const func_info& scope,
        const inst_decl& inst) const {
        shared::Bytecode bytecode;
        shared::inst_code mask = 0;
        std::list<hint> hints;
        size_t start = bytecode.size(), arg_idx = 0;
        const auto& info = _ctx.get_inst(inst.name);

        bytecode.append_value(info.code);
        if (inst.name == "fcall") {
            hints.emplace_back(bytecode.size(), inst.args.front().name);
            bytecode.append_value<shared::machine_word>(0);
        } else {
            for (const auto& arg : inst.args) {
                mask |= (size_t) arg.type << arg_idx * 2;
                arg_idx++;

                switch (arg.type) {
                case ArgumentType::Direct:
                    bytecode.append_value(scope.variables.get_variable(arg.name)->address);
                    break;
                case ArgumentType::Immediate:
                    bytecode.append_value(arg.value, info.bid_depth / 8);
                    break;
                case ArgumentType::IndirectWithDisplacement:
                    bytecode.append_value(scope.variables.get_variable(arg.name)->address);
                    bytecode.append_value(arg.value, shared::machine_word_size);
                    break;
                default:
                    throw std::runtime_error("wrong instruction definition");
                }
            }
        }

        bytecode[start + 3] |= std::byte(mask);

        // TODO: place this in debugger
#ifdef DEBUG_STATUS
        std::cout /*<< bytecode.size() << ' '*/ << inst.name << ' ';
        for (const auto& arg : inst.args) {
            switch (arg.type) {
            case ArgumentType::Direct:
            case ArgumentType::JumpAddress:
                std::cout << arg.name;
                break;
            case ArgumentType::Immediate:
                std::cout << arg.value;
                break;
            case ArgumentType::IndirectWithDisplacement:
                std::cout << arg.name << (arg.value > 0 ? '+' : '-') << std::abs(arg.value);
                break;
            default:
                break;
            }
            std::cout << ' ';
        }
        std::cout << '\n';

        const auto inst_full_data = *reinterpret_cast<const shared::inst_code*>(bytecode.data());
        std::cout << (inst_full_data & 0xFFFF) << ' ';
        for (size_t i = shared::inst_size; i < bytecode.size(); i++)
            std::cout << (int) bytecode[i] << ' ';
        std::cout << '\n';
#endif

        return { bytecode, hints };
    }
}
