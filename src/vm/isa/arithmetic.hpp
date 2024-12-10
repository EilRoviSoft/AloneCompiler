#pragma once

//std
#include <functional>
#include <array>

//alone
#include "instructions.hpp"
#include "vm_context.hpp"

namespace alone::vm::isa {
	template<typename T>
	size_t inst_bop(const context_t& ctx, const std::array<argument_type, 4>& args_metadata, const std::function<T(const T&, const T&)>& pred) {
		size_t offset = inst_code_size;
		T* av;
		T lv, rv;

		if (args_metadata[0] == argument_type::direct) {
			av = ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
		} else if (args_metadata[0] == argument_type::indirect_with_displacement) {
			const auto& val = *ctx.get(ctx.ipx() + offset);
			const auto& displacement = *ctx.get(ctx.ipx() + offset + machine_word_size);
			av = ctx.get<T>(val + displacement);
			offset += machine_word_size * 2;
		}

		if (args_metadata[1] == argument_type::direct) {
			lv = *ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
		} else if (args_metadata[1] == argument_type::immediate) {
			lv = *ctx.get<T>(ctx.ipx() + offset);
			offset += sizeof(T);
		} else if (args_metadata[1] == argument_type::indirect_with_displacement) {
			const auto& val = *ctx.get(ctx.ipx() + offset);
			const auto& displacement = *ctx.get(ctx.ipx() + offset + machine_word_size);
			lv = *ctx.get<T>(val + displacement);
			offset += machine_word_size * 2;
		}

		if (args_metadata[1] == argument_type::direct) {
			rv = *ctx.get<T>(ctx.ipx() + offset);
			offset += machine_word_size;
		} else if (args_metadata[1] == argument_type::immediate) {
			rv = *ctx.get<T>(ctx.ipx() + offset);
			offset += sizeof(T);
		} else if (args_metadata[1] == argument_type::indirect_with_displacement) {
			const auto& val = *ctx.get(ctx.ipx() + offset);
			const auto& displacement = *ctx.get(ctx.ipx() + offset + machine_word_size);
			rv = *ctx.get<T>(val + displacement);
			offset += machine_word_size * 2;
		}

		*av = pred(lv, rv);
		return offset;
	}

	std::list<inst_t> generate_arithmetic_isa();
}
