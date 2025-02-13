#include "executor/instruction_builder.hpp"

namespace amasm::executor {
    InstructionBuilder& InstructionBuilder::set_id(lib::inst_code id) {
        m_product->m_id = id;
        _is_set.id = true;
        return *this;
    }
    InstructionBuilder& InstructionBuilder::set_predicate(InstSignature predicate) {
        m_product->m_predicate = std::move(predicate);
        _is_set.predicate = true;
        return *this;
    }
    InstructionBuilder& InstructionBuilder::set_memory_shift(ptrdiff_t memory_shift) {
        m_product->m_memory_shift = memory_shift;
        return *this;
    }

    bool InstructionBuilder::is_built() const {
        return _is_set.id && _is_set.predicate;
    }

    const char* InstructionBuilder::get_error_message() const {
        return "you have to specify id and predicate of Instruction";
    }
}
