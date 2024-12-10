#include "arithmetic.hpp"

//std
#include <string>

//alone
#include "util.hpp"
#include "virtual_machine.hpp"

#define MAKE_ARITHMETIC_INSTRUCTION(PREFIX, OFFSET, OP, TYPE) std::string(##PREFIX) + std::to_string(sizeof(##TYPE)) + Postfix, Offset + ##OFFSET, 2, sizeof(T), std::bind(inst_bop<##TYPE>, std::placeholders::_1, std::placeholders::_2, ##OP<##TYPE>)

namespace alone::vm::isa {
	template<typename T, char Postfix, size_t Offset>
	std::list<inst_t> generate_datatype_related_arithmetic_instruction() {
		std::list<inst_t> result;

		/*result.emplace_back(std::string("add") + std::to_string(sizeof(T)) + Postfix, Offset + 0x00, 2, sizeof(T), std::bind(inst_bop<T>, std::placeholders::_1, std::placeholders::_2, add<T>));*/
		result.emplace_back(MAKE_ARITHMETIC_INSTRUCTION("add", 0x00, util::add, T));
		result.emplace_back(MAKE_ARITHMETIC_INSTRUCTION("sub", 0x01, util::sub, T));
		result.emplace_back(MAKE_ARITHMETIC_INSTRUCTION("mul", 0x02, util::mul, T));
		result.emplace_back(MAKE_ARITHMETIC_INSTRUCTION("div", 0x03, util::div, T));
		result.emplace_back(MAKE_ARITHMETIC_INSTRUCTION("mod", 0x04, util::mod, T));

		return result;
	}

	std::list<inst_t> generate_arithmetic_isa() {
		std::list<inst_t> result;

		result.append_range(generate_datatype_related_arithmetic_instruction<uint8_t, 'u', 0x140>());

		return result;
	}
}
