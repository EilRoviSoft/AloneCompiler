#include "type_related.hpp"

//std
#include <string>

//alone::util
#include "util.hpp"

//alone::isa
#include "isa/universal.hpp"

namespace ph = std::placeholders;

//TODO: floating numbers isa
namespace alone::isa {
    template<typename T, char Postfix, size_t Offset>
    std::list<lib::inst_t> generate_universal_type_related_isa() {
        std::list<lib::inst_t> result;

        result.append_range(std::initializer_list {
            lib::make_instruction(
                "add" + std::to_string(sizeof(T) * 8) + Postfix,
                Offset + 0x0,
                3,
                sizeof(T) * 8,
                std::bind(inst_b23<T>, ph::_1, ph::_2, util::call_add<T>)
            ),
            lib::make_instruction(
                "sub" + std::to_string(sizeof(T) * 8) + Postfix,
                Offset + 0x1,
                3,
                sizeof(T) * 8,
                std::bind(inst_b23<T>, ph::_1, ph::_2, util::call_sub<T>)
            ),
            lib::make_instruction(
                "mul" + std::to_string(sizeof(T) * 8) + Postfix,
                Offset + 0x2,
                3,
                sizeof(T) * 8,
                std::bind(inst_b23<T>, ph::_1, ph::_2, util::call_mul<T>)
            ),
            lib::make_instruction(
                "div" + std::to_string(sizeof(T) * 8) + Postfix,
                Offset + 0x3,
                3,
                sizeof(T) * 8,
                std::bind(inst_b23<T>, ph::_1, ph::_2, util::call_div<T>)
            ),
            lib::make_instruction(
                "mod" + std::to_string(sizeof(T) * 8) + Postfix,
                Offset + 0x4,
                3,
                sizeof(T) * 8,
                std::bind(inst_b23<T>, ph::_1, ph::_2, util::call_mod<T>)
            ),
            lib::make_instruction(
                "inc" + std::to_string(sizeof(T) * 8) + Postfix,
                Offset + 0x5,
                2,
                sizeof(T) * 8,
                std::bind(inst_u12<T>, ph::_1, ph::_2, util::call_inc<T>)
            ),
            lib::make_instruction(
                "dec" + std::to_string(sizeof(T) * 8) + Postfix,
                Offset + 0x6,
                2,
                sizeof(T) * 8,
                std::bind(inst_u12<T>, ph::_1, ph::_2, util::call_dec<T>)
            ),
            lib::make_instruction(
                "cmp" + std::to_string(sizeof(T) * 8) + Postfix,
                Offset + 0x7,
                2,
                sizeof(T) * 8,
                std::bind(inst_cmp<T>, ph::_1, ph::_2)
            )
        });

        return result;
    }
    template<typename T, char Postfix, size_t Offset>
    std::list<lib::inst_t> generate_signed_type_related_isa() {
        std::list<lib::inst_t> result;

        result.append_range(generate_universal_type_related_isa<T, Postfix, Offset>());
        result.push_back(lib::make_instruction(
            std::string("neg") + std::to_string(sizeof(T) * 8) + Postfix,
            Offset + 0x8,
            3,
            sizeof(T) * 8,
            std::bind(inst_u12<T>, ph::_1, ph::_2, util::call_neg<T>)
        ));

        return result;
    }

    std::list<lib::inst_t> generate_type_related_isa() {
        std::list<lib::inst_t> result;

        result.append_range(generate_universal_type_related_isa<uint8_t, 'u', 0x140>());
        result.append_range(generate_universal_type_related_isa<uint16_t, 'u', 0x150>());
        result.append_range(generate_universal_type_related_isa<uint32_t, 'u', 0x160>());
        result.append_range(generate_universal_type_related_isa<uint64_t, 'u', 0x170>());
        result.append_range(generate_signed_type_related_isa<int8_t, 'i', 0x180>());
        result.append_range(generate_signed_type_related_isa<int16_t, 'i', 0x190>());
        result.append_range(generate_signed_type_related_isa<int32_t, 'i', 0x1A0>());
        result.append_range(generate_signed_type_related_isa<int64_t, 'i', 0x1B0>());

        return result;
    }
}
