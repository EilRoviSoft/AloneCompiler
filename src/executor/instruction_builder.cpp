#include "executor/instruction_builder.hpp"

//std
#include <stdexcept>

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
        return _is_set.id
            && _is_set.predicate;
    }

    Instruction&& InstructionBuilder::get_product() {
        if (!is_built())
            throw std::runtime_error("you have to specify id and predicate of Instruction");

        return IBuilder::get_product();
    }
}
