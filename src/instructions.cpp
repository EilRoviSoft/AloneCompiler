#include "instructions.hpp"

//alone::isa
#include "isa/system.hpp"
#include "isa/size_related.hpp"
#include "isa/type_related.hpp"

namespace alone::lib {
    std::unordered_map<std::string, inst_ptr> instructions_by_name;
    std::unordered_map<inst_code_t, inst_ptr> instructions_by_code;

    inst_t make_instruction(std::string name,
                            const inst_code_t& code,
                            const size_t& min_args_count,
                            const size_t& max_args_count,
                            const size_t& bit_depth,
                            inst_func_t pred) {
        return inst_t {
            .name = std::move(name),
            .code = code,
            .min_args_count = min_args_count,
            .max_args_count = max_args_count,
            .bit_depth = bit_depth,
            .pred = std::move(pred)
        };
    }

    void init_instruction(std::string name,
                          inst_code_t code,
                          size_t min_args_count,
                          size_t max_args_count,
                          size_t bit_depth,
                          inst_func_t pred) {
        auto ptr = std::make_shared<inst_t>(
            std::move(name),
            code,
            min_args_count,
            max_args_count,
            bit_depth,
            std::move(pred)
        );
        instructions_by_name.emplace(ptr->name, ptr);
        instructions_by_code.emplace(ptr->code, ptr);
    }
    void init_instruction(inst_t&& inst) {
        auto ptr = std::make_shared<inst_t>(std::move(inst));
        instructions_by_name.emplace(ptr->name, ptr);
        instructions_by_code.emplace(ptr->code, ptr);
    }

    void init_isa() {
        const std::initializer_list<std::function<std::list<inst_t>()>> isa_predicates = {
            isa::generate_system_isa,
            isa::generate_size_related_isa,
            isa::generate_type_related_isa
        };

        for (const auto& pred : isa_predicates) {
            auto isa = pred();
            while (!isa.empty()) {
                init_instruction(std::move(isa.front()));
                isa.pop_front();
            }
        }
    }
}
