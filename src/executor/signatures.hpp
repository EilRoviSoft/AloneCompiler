#pragma once

//std
#include <functional>

namespace amasm::executor {
    class Context;
    class VirtualMachine;

    using InstSignature = std::function<void(const Context&, lib::args_data)>;
    using CallSignature = std::function<void(const Context&)>;
    using StepperSignature = std::function<void(VirtualMachine*, const Context&)>;
}
