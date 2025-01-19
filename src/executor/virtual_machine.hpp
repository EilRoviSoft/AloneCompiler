#pragma once

//std
#include <array>
#include <unordered_map>

//shared
#include "shared/bytecode.hpp"
#include "shared/types.hpp"

//executor
#include "executor/constants.hpp"

namespace amasm::executor {
    class Context;

    using inst_func = std::function<ptrdiff_t(const Context&, const shared::args_data&)>;

    class VirtualMachine {
        friend class Context;

    public:
        void init_instructions();

        void exec(Context& ctx, shared::Bytecode program);

    private:
        std::unordered_map<shared::inst_code, inst_func> _instructions;

        std::array<std::byte, mframe_size> _mframe;
        std::unordered_map<shared::address, std::vector<std::byte>> _dframe;
    };
}
