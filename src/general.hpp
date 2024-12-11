#pragma once

//std
#include <array>
#include <bitset>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace alone::lib {
	//data_types

	using inst_code_t = uint32_t;
	using byte_array_t = std::vector<std::byte>;
	using address_t = uint64_t;
	using machine_word_t = uint64_t;
	using flags_t = std::bitset<64>;

	enum class argument_type : uint8_t {
		empty = 0,
		direct,
		immediate,
		indirect_with_displacement
	};
	using arguments_metadata_t = std::array<argument_type, 4>;

	enum class literal_type : uint8_t {
		none = 0,
		word,
		binary, decimal, hexadecimal, floating
	};

	enum class regs_set : uint8_t {
		asx = 0 * sizeof(machine_word_t), //Address System register
		rsx = 1 * sizeof(machine_word_t), //Result System register
		lox = 2 * sizeof(machine_word_t), //Left Operand register
		rox = 3 * sizeof(machine_word_t), //Right Operand register
		ipx = 4 * sizeof(machine_word_t), //Instruction Pointer
		bpx = 5 * sizeof(machine_word_t), //Stack Pointer
		spx = 6 * sizeof(machine_word_t), //Base Pointer
		flags = 7 * sizeof(machine_word_t), //Flags register
		grx = 8 * sizeof(machine_word_t), //General Registers start
	};

	enum class flags_set : uint8_t {
		rf = 0, //is program running flag
		zf = 1, //zero flag
		sf = 2, //sign flag
		cf = 3, //carry flag
		of = 4, //overflow flag
	};

	enum class memory_type : uint8_t {
		mframe, //registers + application + stack
		dframe, //pointers
		eframe, //external memory
		sframe, //static memory
	};

	template<typename... Ts> struct overloaded : Ts... {
		using Ts::operator()...;
	};

	template<typename T>
	struct array_t {
		size_t size;
		T* ptr;
	};

	//objects

	inline auto string_hasher = std::hash<std::string>();
	constexpr size_t machine_word_size = sizeof(uint64_t);
	constexpr size_t inst_code_size = sizeof(inst_code_t);
	constexpr size_t registers_size = 256;
	constexpr size_t system_registers_size = registers_size - (uint64_t) regs_set::grx;
	constexpr size_t general_registers_size = registers_size - system_registers_size;
	constexpr size_t mframe_size = 1 << 16;
	constexpr size_t dframe_size = 16;

	//functions

	constexpr bool is_alpha(char c);
	constexpr bool is_binary(char c);
	constexpr bool is_numeric(char c);
	constexpr bool is_hexadecimal(char c);
	constexpr bool is_alpha_numeric(char c);
	constexpr bool is_whitespace(char c);

	literal_type check_type(const std::string& l);

	std::string gen_str(char a, char b);

	std::tuple<address_t, memory_type> decompose_address(address_t address);
	std::tuple<inst_code_t, arguments_metadata_t> decompose_instruction(inst_code_t instruction);
}
