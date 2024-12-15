#pragma once

//std
#include <fstream>
#include <string>

//alone::amasm::info
#include "amasm/context.hpp"

namespace alone::amasm {
    class Scanner {
    public:
        std::string scan(const AmasmContext& ctx, std::fstream& file) const;
        std::string scan(const AmasmContext& ctx, const std::string& input) const;
    };
}
