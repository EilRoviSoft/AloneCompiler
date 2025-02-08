#pragma once

//std
#include <list>

//library
#include "library/factory.hpp"

//executor
#include "executor/instruction.hpp"

namespace amasm::executor {
	class IsaFactory : public lib::IFactory<std::list<Instruction>> {
	public:

	};
}
