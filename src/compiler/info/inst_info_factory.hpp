#pragma once

//std
#include <list>

//library
#include "library/factory.hpp"

//compiler_info
#include "compiler/info/instruction.hpp"

namespace amasm::compiler {
    class InstInfoFactory : public lib::IFactory<std::list<InstInfo>> {
    public:
        InstInfoFactory& generate_system();
        template<typename T, size_t TOffset>
        InstInfoFactory& generate_universal_size();
        template<typename T, char TPostfix, size_t TOffset>
        InstInfoFactory& generate_universal_type();
        template<typename T, char TPostfix, size_t TOffset>
        InstInfoFactory& generate_signed_type();

        InstInfoFactory& generate_info();
    };
}
