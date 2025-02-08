#include "compiler/translator.hpp"

//std
#include <algorithm>
#include <list>
#include <tuple>

//compiler_info
#include "compiler/info/function.hpp"
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_container.hpp"

namespace amasm::compiler {
    std::tuple<lib::Bytecode, std::list<std::tuple<size_t, std::string>>> generate_inst_bytecode(
        const ScopeProxy& scope,
        const InstDecl& inst) {
        lib::Bytecode bytecode;
        lib::inst_code mask = 0;
        std::list<std::tuple<size_t, std::string>> hints;

        bytecode.append_value(inst.info().code());
        if (inst.info().name() == "fcall") {
            hints.emplace_back(bytecode.size(), inst.argument(0).name);
            bytecode.append_value<lib::machine_word>(0);
        } else {
            size_t arg_idx = 0;
            for (size_t i = 0; i < inst.arguments_count(); i++) {
                const auto& arg = inst.argument(i);
                mask |= (size_t) arg.type << arg_idx * 2;
                arg_idx++;

                switch (arg.type) {
                case ArgumentType::Direct:
                    bytecode.append_value(scope.get_variable(arg.name).address());
                    break;
                case ArgumentType::Immediate:
                    bytecode.append_value(arg.value, inst.info().bit_depth() / 8);
                    break;
                case ArgumentType::IndirectWithDisplacement:
                    bytecode.append_value(scope.get_variable(arg.name).address());
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

    // Translator

    Translator::Translator(Context& ctx) :
        _ctx(ctx) {
    }

    lib::Bytecode Translator::translate(ScopeContainer container) const {
        lib::Bytecode result;
        std::unordered_map<std::string, size_t> labels;
        std::list<std::tuple<size_t, std::string>> hints;
        ScopeProxy scopes = container;
        std::vector<Function> functions;

        auto temp_container = scopes.get_all_functions();
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
                auto [new_bytecode, new_hints] = generate_inst_bytecode(scopes, it.line(i));
                for (auto& [offset, name] : new_hints)
                    offset += result.size();
                hints.append_range(new_hints);
                result.append_sequence(new_bytecode);
            }
        }

        result.set(labels.at("@main()"), 0);
        for (const auto& [offset, name] : hints)
            result.set(labels[name] + lib::registers_size, offset);

        return result;
    }
}
