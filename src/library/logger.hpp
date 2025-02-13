#pragma once

//std
#include <ostream>

namespace amasm::lib {
    class Logger {
    public:
        enum Channel : uint8_t {
            General = 0,
            Error,
            Output,
            Max
        };

        static void init();
        static std::ostream& channel(Channel id);
    };
}
