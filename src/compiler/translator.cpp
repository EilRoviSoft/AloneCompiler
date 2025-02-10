#include "compiler/translator.hpp"

//std
#include <algorithm>

//compiler_info
#include "compiler/info/function.hpp"
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_container.hpp"

namespace amasm::compiler {
    lib::Bytecode Translator::translate(const ScopeContainer& container) {
        lib::Bytecode result;
        std::unordered_map<std::string, size_t> labels;
        std::list<hint> hints;
        std::vector<Function> functions;

        _scopes = container;
        auto temp_container = _scopes.get_all_functions();
        functions.reserve(temp_container.size());
        for (auto&& it : temp_container)
            functions.emplace_back(*it);

        std::ranges::sort(functions, std::ranges::less(), &Function::scope_id);

        // adds total size of program to the front of the code
        for (const auto& it : functions) {
            result.append_value<lib::machine_word>(it.arguments_size());
            result.append_value<lib::machine_word>(it.return_type().size());
            labels.emplace(it.fullname(), result.size());

            for (size_t i = 0; i < it.lines_size(); i++) {
                auto [new_bytecode, new_hints] = _generate_inst_bytecode(it.line(i));
                for (auto& hint : new_hints)
                    hint.offset += result.size();
                hints.append_range(new_hints);
                result.append_sequence(new_bytecode);
            }
        }

        result.set(labels.at("@main()"), 0);
        for (const auto& hint : hints)
            result.set(labels[hint.name] + lib::registers_size, hint.offset);

        return result;
    }

    std::tuple<lib::Bytecode, std::list<Translator::hint>> Translator::_generate_inst_bytecode(const InstDecl& inst) {
        lib::Bytecode bytecode;
        lib::inst_code mask = 0;
        std::list<hint> hints;

        bytecode.append_value(inst.info().code());
        if (inst.info().name() == "fcall") {
            hints.emplace_back(inst.argument(0).name, bytecode.size());
            bytecode.append_value<lib::machine_word>(0);
        } else {
            size_t arg_idx = 0;
            for (size_t i = 0; i < inst.arguments_count(); i++) {
                const auto& arg = inst.argument(i);
                mask |= (size_t) arg.type << arg_idx * 2;
                arg_idx++;

                switch (arg.type) {
                case ArgumentType::Direct:
                    bytecode.append_value(_scopes.get_variable(arg.name).address());
                    break;
                case ArgumentType::Immediate:
                    bytecode.append_value(arg.value, inst.info().bit_depth() / 8);
                    break;
                case ArgumentType::IndirectWithDisplacement:
                    bytecode.append_value(_scopes.get_variable(arg.name).address());
                    bytecode.append_value(arg.value, lib::machine_word_size);
                    break;
                default:
                    throw std::runtime_error("wrong instruction definition");
                }
            }
        }

        bytecode[3] |= std::byte(mask);

        return { std::move(bytecode), std::move(hints) };
    }
}
