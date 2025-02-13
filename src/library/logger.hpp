#pragma once

//std
#include <ostream>

namespace amasm::lib {
    class Logger {
    public:
        static void init(std::ostream& out) { _out = &out; }

        static std::ostream& out() { return *_out; }

    private:
        static std::ostream* _out;
    };
}
