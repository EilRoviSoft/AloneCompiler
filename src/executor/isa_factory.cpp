#include "executor/isa_factory.hpp"

//std
#include <array>

//library
#include "library/general_functions.hpp"

//executor
#include "executor/instruction_builder.hpp"
#include "executor/isa_funcs.hpp"

namespace amasm::executor {
    // generators

    using B = InstructionBuilder;

    std::list<Instruction> gisa_system() {
        return {
            B().set_id(0x0)
               .set_predicate(isa::halt)
               .get_product(),
            B().set_id(0x2)
               .set_predicate(isa::fcall)
               .set_memory_shift(8)
               .get_product(),
            B().set_id(0x3)
               .set_predicate(isa::ret)
               .set_memory_shift(-8)
               .get_product()
        };
    }
    template<typename T, size_t Offset>
    std::list<Instruction> gisa_generic_size_related() {
        return {
            B().set_id(Offset + 0x0)
               .set_predicate(isa::inst_mov<T>)
               .get_product(),
            B().set_id(Offset + 0x1)
               .set_predicate(isa::inst_push<T>)
               .set_memory_shift(sizeof(T))
               .get_product(),
            B().set_id(Offset + 0x2)
               .set_predicate(isa::inst_pop<T>)
               .set_memory_shift(-sizeof(T))
               .get_product(),
            B().set_id(Offset + 0x3)
               .set_predicate(isa::inst_u12<T, std::bit_not<T>>)
               .get_product(),
            B().set_id(Offset + 0x4)
               .set_predicate(isa::inst_b23<T, std::bit_and<T>>)
               .get_product(),
            B().set_id(Offset + 0x5)
               .set_predicate(isa::inst_b23<T, std::bit_or<T>>)
               .get_product(),
            B().set_id(Offset + 0x6)
               .set_predicate(isa::inst_b23<T, std::bit_xor<T>>)
               .get_product(),
            B().set_id(Offset + 0x7)
               .set_predicate(isa::inst_b23<T, lib::bit_shl<T>>)
               .get_product(),
            B().set_id(Offset + 0x8)
               .set_predicate(isa::inst_b23<T, lib::bit_shr<T>>)
               .get_product()
        };
    }
    std::list<Instruction> gisa_size_related() {
        std::list<Instruction> result;

        result.append_range(gisa_generic_size_related<uint8_t, 0x100>());
        result.append_range(gisa_generic_size_related<uint16_t, 0x110>());
        result.append_range(gisa_generic_size_related<uint32_t, 0x120>());
        result.append_range(gisa_generic_size_related<uint64_t, 0x130>());

        return result;
    }
    template<typename T, size_t Offset>
    std::list<Instruction> gisa_generic_type_related() {
        return {
            B().set_id(Offset + 0x0)
               .set_predicate(isa::inst_b23<T, std::plus<T>>)
               .get_product(),
            B().set_id(Offset + 0x1)
               .set_predicate(isa::inst_b23<T, std::minus<T>>)
               .get_product(),
            B().set_id(Offset + 0x2)
               .set_predicate(isa::inst_b23<T, std::multiplies<T>>)
               .get_product(),
            B().set_id(Offset + 0x3)
               .set_predicate(isa::inst_b23<T, std::divides<T>>)
               .get_product(),
            B().set_id(Offset + 0x4)
               .set_predicate(isa::inst_b23<T, std::modulus<T>>)
               .get_product(),
            B().set_id(Offset + 0x5)
               .set_predicate(isa::inst_cmp<T>)
               .get_product()
        };
    }
    template<typename T, size_t Offset>
    std::list<Instruction> gisa_signed_type_related() {
        std::list<Instruction> result;

        result.append_range(gisa_generic_type_related<T, Offset>());
        result.emplace_back(
            B().set_id(Offset + 0x6)
               .set_predicate(isa::inst_u12<T, std::negate<T>>)
               .get_product()
        );

        return result;
    }
    std::list<Instruction> gisa_type_related() {
        std::list<Instruction> result;

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

    // IsaFactory

    IsaFactory& IsaFactory::generate_isa() {
        constexpr std::array funcs = {
            gisa_system,
            gisa_size_related,
            gisa_type_related
        };

        for (const auto& f : funcs)
            m_product->append_range(f());

        return *this;
    }
}
