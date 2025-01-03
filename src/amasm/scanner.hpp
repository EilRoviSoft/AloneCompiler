#pragma once

//std
#include <fstream>
#include <string>

//alone::amasm::info
#include "amasm/info/context.hpp"

namespace alone::amasm {
    class Scanner {
    public:
        explicit Scanner(Context& ctx);

        std::string scan(std::fstream& file) const;
        std::string scan(const std::string& input) const;

    private:
        Context& _ctx;
    };
}
