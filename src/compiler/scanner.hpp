#pragma once

//std
#include <fstream>
#include <string>

namespace amasm::compiler {
    class Scanner {
    public:
        static std::string scan_from_file(std::fstream& file);
        static std::string scan_from_file(const std::string& filename);

        static std::string format_string(const std::string& input);
    };
}
