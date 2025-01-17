#pragma once

//std
#include <array>
#include <memory>
#include <unordered_map>

//shared
#include "shared/bytecode.hpp"
#include "shared/types.hpp"

//executor
#include "executor/constants.hpp"

namespace amasm::executor {
	class Context;
	using SharedContext = std::shared_ptr<Context>;

	class VirtualMachine {
		friend class Context;
	public:
		void exec(const SharedContext& ctx, const shared::Bytecode& program);

	private:
		std::array<std::byte, mframe_size> _mframe;
		std::unordered_map<shared::Address, std::vector<std::byte>> _dframe;
	};
}
