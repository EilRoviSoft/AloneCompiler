#pragma once

//std
#include <array>
#include <cstddef>

//alone
#include "general.hpp"
#include "vm_context.hpp"

namespace alone::vm {
    class VirtualMachine {
        friend context_t;

    public:
        context_t ctx;

        VirtualMachine();

        void exec(const lib::byte_array_t& program);

    private:
        std::array<std::byte, lib::mframe_size> _p0;
        std::array<lib::array_t<std::byte>, lib::dframe_size> _p1;
    };
}
