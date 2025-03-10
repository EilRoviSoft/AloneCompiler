#include "compiler/translator.hpp"

//std
#include <algorithm>
#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//compiler_info
#include "compiler/info/function.hpp"
#include "compiler/info/instruction.hpp"
#include "compiler/info/scope_container.hpp"

namespace amasm::compiler::translator {
    struct hint {
        std::string name;
        size_t offset;
    };

    std::tuple<lib::Bytecode, std::list<hint>> generate_inst_bytecode(
        ConstScopeProxy scopes,
        const InstDecl& inst,
        size_t scope_id) {
        lib::Bytecode bytecode;
        lib::inst_code mask = 0;
        std::list<hint> hints;

        bytecode.append_value(inst.info().code());
        if (inst.info().name() == "fcall") {
            hints.emplace_back(inst.argument(0).name, bytecode.size());
            bytecode.append_value<lib::machine_word>(0);
        } else if (inst.info().name() == "ncall") {
            bytecode.append_value<lib::machine_word>(inst.argument(0).abs_value);
        } else {
            size_t arg_idx = 0;
            for (size_t i = 0; i < inst.arguments_count(); i++) {
                const auto& arg = inst.argument(i);
                mask |= (size_t) arg.type << arg_idx * 2;
                arg_idx++;

                switch (arg.type) {
                case AddressType::Relative:
                    bytecode.append_value(scopes.get_variable(arg.name, scope_id).address().abs_value);
                    break;
                case AddressType::Fixed:
                    bytecode.append_value(arg.abs_value, inst.info().bit_depth() / 8);
                    break;
                case AddressType::RelativeWithDiff:
                    bytecode.append_value(scopes.get_variable(arg.name, scope_id).address().abs_value);
                    bytecode.append_value(arg.sign_value, lib::machine_word_size);
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

namespace amasm::compiler {
    lib::Bytecode Translator::translate(const ScopeContainer& container) {
        lib::Bytecode bytecode;
        std::unordered_map<std::string, size_t> labels;
        std::list<translator::hint> hints;
        std::vector<Function> functions;
        ConstScopeProxy scopes = container;

        auto temp_container = scopes.get_all_functions();
        functions.reserve(temp_container.size());
        for (auto&& it : temp_container)
            functions.emplace_back(*it);

        std::ranges::sort(functions, std::ranges::less(), &Function::scope_id);

        // adds total size of program to the front of the code
        for (const auto& it : functions) {
            bytecode.append_value<lib::machine_word>(it.calc_arguments_size());
            bytecode.append_value<lib::machine_word>(it.return_type().size());
            labels.emplace(it.fullname(), bytecode.size());

            for (size_t i = 0; i < it.lines_size(); i++) {
                const auto& item = it.line(i);
                if (item.type() == LineVariant::Inst) {
                    auto [new_bytecode, new_hints] = translator::generate_inst_bytecode(
                        scopes,
                        it.line(i).inst(),
                        it.scope_id()
                    );
                    for (auto& hint : new_hints)
                        hint.offset += bytecode.size();

                    hints.append_range(new_hints);
                    bytecode.append_sequence(new_bytecode);
                } else if (item.type() == LineVariant::Label) {
                    labels.emplace(it.line(i).label(), bytecode.size());
                } else
                    throw std::runtime_error("how the fuck did you get here?");
            }
        }

        bytecode.set(labels.at("@main()"), 0);
        for (const auto& hint : hints)
            bytecode.set(labels[hint.name] + lib::registers_size, hint.offset);

        return bytecode;
    }
}
