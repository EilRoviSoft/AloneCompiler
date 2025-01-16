#pragma once

//std
#include <fstream>
#include <string>

//compiler
#include "compiler/info/context.hpp"

namespace amasm::compiler {
    class Scanner {
    public:
        explicit Scanner(Context& ctx);

        std::string scan_from_file(std::fstream& file) const;
        std::string scan_from_file(const std::string& filename) const;

        std::string scan_from_string(const std::string& input) const;

    private:
        Context& _ctx;
    };
}
