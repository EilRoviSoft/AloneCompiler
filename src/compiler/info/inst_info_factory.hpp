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
        InstInfoFactory& generate_info();
    };
}
