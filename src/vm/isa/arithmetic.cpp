#include "arithmetic.hpp"

//std
#include <string>

//alone
#include "util.hpp"
#include "virtual_machine.hpp"

#define ARITHMETIC_BODY(PREFIX, OFFSET, TYPE) std::string(##PREFIX) + std::to_string(sizeof(##TYPE) * 8) + Postfix, Offset + ##OFFSET, 2, sizeof(T)
#define MAKE_INST_B23(PREFIX, OFFSET, OP, TYPE) ARITHMETIC_BODY(PREFIX, OFFSET, TYPE), std::bind(inst_b23<##TYPE>, std::placeholders::_1, std::placeholders::_2, ##OP<##TYPE>)
#define MAKE_INST_U12(PREFIX, OFFSET, OP, TYPE) ARITHMETIC_BODY(PREFIX, OFFSET, TYPE), std::bind(inst_u12<##TYPE>, std::placeholders::_1, std::placeholders::_2, ##OP<##TYPE>)
#define MAKE_INST_CMP(PREFIX, OFFSET, TYPE) ARITHMETIC_BODY(PREFIX, OFFSET, TYPE), std::bind(inst_cmp<##TYPE>, std::placeholders::_1, std::placeholders::_2)

//TODO: floating numbers isa
namespace alone::vm::isa {
	template<typename T, char Postfix, size_t Offset>
	std::list<inst_t> generate_universal_arithmetic_isa() {
		std::list<inst_t> result;

		result.emplace_back(MAKE_INST_B23("add", 0x00, util::add, T));
		result.emplace_back(MAKE_INST_B23("sub", 0x01, util::sub, T));
		result.emplace_back(MAKE_INST_B23("mul", 0x02, util::mul, T));
		result.emplace_back(MAKE_INST_B23("div", 0x03, util::div, T));
		result.emplace_back(MAKE_INST_B23("mod", 0x04, util::mod, T));
		result.emplace_back(MAKE_INST_U12("inc", 0x05, util::inc, T));
		result.emplace_back(MAKE_INST_U12("dec", 0x06, util::dec, T));
		result.emplace_back(MAKE_INST_CMP("cmp", 0x07, T));

		return result;
	}
	template<typename T, char Postfix, size_t Offset>
	std::list<inst_t> generate_signed_arithmetic_isa() {
		std::list<inst_t> result;

		result.append_range(generate_universal_arithmetic_isa<T, Postfix, Offset>());
		result.emplace_back(MAKE_INST_U12("neg", 0x08, util::neg, T));

		return result;
	}

	std::list<inst_t> generate_arithmetic_isa() {
		std::list<inst_t> result;

		result.append_range(generate_universal_arithmetic_isa<uint8_t, 'u', 0x140>());
		result.append_range(generate_universal_arithmetic_isa<uint16_t, 'u', 0x150>());
		result.append_range(generate_universal_arithmetic_isa<uint32_t, 'u', 0x160>());
		result.append_range(generate_universal_arithmetic_isa<uint64_t, 'u', 0x170>());
		result.append_range(generate_signed_arithmetic_isa<int8_t, 'i', 0x180>());
		result.append_range(generate_signed_arithmetic_isa<int16_t, 'i', 0x190>());
		result.append_range(generate_signed_arithmetic_isa<int32_t, 'i', 0x1A0>());
		result.append_range(generate_signed_arithmetic_isa<int64_t, 'i', 0x1B0>());

		return result;
	}
}
