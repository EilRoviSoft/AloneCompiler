#pragma once

//std
#include <list>

//alone
#include "instructions.hpp"

namespace alone::isa {
    std::list<lib::inst_t> generate_type_related_isa();
}
