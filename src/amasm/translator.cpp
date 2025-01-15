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

namespace alone::amasm {
    Translator::Translator(Context& ctx) :
        _ctx(ctx) {
    }

    shared::Bytecode Translator::translate(composed_funcs_t composed) const {
        shared::Bytecode bytecode;
        std::unordered_map<std::string, size_t> labels;
        std::list<std::tuple<size_t, std::string>> hints;
        size_t summed_size = 0;

        // adds total size of program to the front of the code
        bytecode.append_value<shared::machine_word_t>(0);
        while (!composed.empty()) {
            const auto func = std::move(composed.front());
            composed.pop();

            bytecode.append_value<shared::machine_word_t>(shared::for_each_by_rule(
                func.args,
                0,
                std::plus {},
                [](const datatype_ptr& ptr) { return ptr->size; }
            ));
            bytecode.append_value<shared::machine_word_t>(func.return_type->size);

            auto old_size = bytecode.size();

            for (const auto& inst : func.lines) {
                shared::inst_code_t mask = 0;
                size_t start = bytecode.size(), arg_idx = 0;
                const auto& info = _ctx.get_inst(inst.name);

                bytecode.append_value(info.code);
                if (inst.name == "fcall") {
                    hints.emplace_back(bytecode.size(), inst.args.front().name);
                    bytecode.append_value<shared::machine_word_t>(0);
                } else {
                    for (const auto& arg : inst.args) {
                        mask |= (size_t) arg.type << arg_idx * 2;
                        arg_idx++;

                        switch (arg.type) {
                        case Arguments::Direct:
                            bytecode.append_value(func.variables.get_variable(arg.name)->address);
                            break;

                        case Arguments::Immediate:
                            bytecode.append_value(arg.value, info.bid_depth / 8);
                            break;

                        case Arguments::IndirectWithDisplacement:
                            bytecode.append_value(func.variables.get_variable(arg.name)->address);
                            bytecode.append_value(arg.value, info.bid_depth / 8);
                            break;

                        default:
                            throw std::runtime_error("wrong instruction definition");
                        }
                    }
                }

                bytecode[start] |= std::byte(mask);

                // TODO: place this in debugger
#ifdef DEBUG_STATUS
                if (bytecode.size() > old_size) {
                    auto temp = bytecode.to_vector();

                    std::cout << inst.name << ' ';
                    for (auto arg : inst.args) {
                        switch (arg.type) {
                        case Arguments::Direct:
                        case Arguments::JumpAddress:
                            std::cout << arg.name;
                            break;
                        case Arguments::Immediate:
                            std::cout << arg.value;
                            break;
                        case Arguments::IndirectWithDisplacement:
                            std::cout << arg.name << (arg.value > 0 ? '+' : '-') << std::abs(arg.value);
                            break;
                        }
                        std::cout << ' ';
                    }
                    std::cout << '\n';

                    for (size_t i = old_size; i < bytecode.size(); i++)
                        std::cout << (int) bytecode[i] << ' ';
                    std::cout << '\n';
                    old_size = bytecode.size();
                }
#endif
            }
        }

        return bytecode;
    }
}
