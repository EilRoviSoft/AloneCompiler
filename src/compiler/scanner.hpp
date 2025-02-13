#pragma once

//std
#include <fstream>
#include <string>

namespace amasm::compiler {
    class Scanner {
    public:
        std::string scan_from_file(std::fstream& file);
        std::string scan_from_file(const std::string& filename);

        std::string format_string(const std::string& input);
    };
}
