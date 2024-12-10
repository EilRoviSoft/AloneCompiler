#pragma once

//std
#include <array>
#include <functional>

//alone
#include "error_codes.hpp"
#include "instructions.hpp"
#include "vm_context.hpp"

namespace alone::vm::isa {
	template<typename T>
	size_t inst_b23(const context_t& ctx, const std::array<argument_type, 4>& args_metadata, const std::function<T(const T&, const T&)>& pred) {
		size_t offset = inst_code_size;
		bool is_reduced = false;
		T* a0;
		T a1, a2;

		switch (args_metadata[0]) {
		case argument_type::direct:
			a0 = ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
			break;
		case argument_type::indirect_with_displacement:
			a0 = ctx.get_with_displacement<T>(ctx.ipx() + offset);
			offset += machine_word_size * 2;
			break;
		default:
			throw ISA_WRONG_ARGS_METADATA;
		}

		switch (args_metadata[1]) {
		case argument_type::direct:
			a1 = *ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
			break;
		case argument_type::immediate:
			a1 = *ctx.get<T>(ctx.ipx() + offset);
			offset += sizeof(T);
			break;
		case argument_type::indirect_with_displacement:
			a1 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
			offset += machine_word_size * 2;
			break;
		default:
			throw ISA_WRONG_ARGS_METADATA;
		}

		switch (args_metadata[2]) {
		case argument_type::direct:
			a2 = *ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
			break;
		case argument_type::immediate:
			a2 = *ctx.get<T>(ctx.ipx() + offset);
			offset += sizeof(T);
			break;
		case argument_type::indirect_with_displacement:
			a2 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
			offset += machine_word_size * 2;
			break;
		case argument_type::empty:
			is_reduced = true;
			break;
		}

		
		*a0 = is_reduced ? pred(*a0, a1) : pred(a1, a2);

		return offset;
	}

	template<typename T>
	size_t inst_u12(const context_t& ctx, const std::array<argument_type, 4>& args_metadata, const std::function<T(const T&)>& pred) {
		size_t offset = inst_code_size;
		bool is_reduced = false;
		T* a0;
		T a1;

		switch (args_metadata[0]) {
		case argument_type::direct:
			a0 = ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
			break;
		case argument_type::indirect_with_displacement:
			a0 = ctx.get_with_displacement<T>(ctx.ipx() + offset);
			offset += machine_word_size * 2;
			break;
		default:
			throw ISA_WRONG_ARGS_METADATA;
		}

		switch (args_metadata[1]) {
		case argument_type::direct:
			a1 = *ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
			break;
		case argument_type::immediate:
			a1 = *ctx.get<T>(ctx.ipx() + offset);
			offset += sizeof(T);
			break;
		case argument_type::indirect_with_displacement:
			a1 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
			offset += machine_word_size * 2;
			break;
		case argument_type::empty:
			is_reduced = true;
			break;
		}

		*a0 = is_reduced ? pred(*a0) : pred(a1);

		return offset;
	}

	template<typename T>
	size_t inst_cmp(const context_t& ctx, const std::array<argument_type, 4>& args_metadata) {
		size_t offset = inst_code_size;
		T a0, a1;

		switch (args_metadata[0]) {
		case argument_type::direct:
			a0 = *ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
			break;
		case argument_type::immediate:
			a0 = *ctx.get<T>(ctx.ipx() + offset);
			offset += sizeof(T);
			break;
		case argument_type::indirect_with_displacement:
			a0 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
			offset += machine_word_size * 2;
			break;
		default:
			throw ISA_WRONG_ARGS_METADATA;
		}

		switch (args_metadata[1]) {
		case argument_type::direct:
			a1 = *ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
			break;
		case argument_type::immediate:
			a1 = *ctx.get<T>(ctx.ipx() + offset);
			offset += sizeof(T);
			break;
		case argument_type::indirect_with_displacement:
			a1 = *ctx.get_with_displacement<T>(ctx.ipx() + offset);
			offset += machine_word_size * 2;
			break;
		default:
			throw ISA_WRONG_ARGS_METADATA;
		}

		ctx.flags()[(size_t) flags_set::zf] = !(a0 - a1);
		ctx.flags()[(size_t) flags_set::sf] = (a0 - a1) >> (sizeof(T) * 8 - 1);

		return offset;
	}

	std::list<inst_t> generate_arithmetic_isa();
}
