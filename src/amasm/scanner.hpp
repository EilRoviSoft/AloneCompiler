﻿#pragma once

//std
#include <fstream>
#include <string>

namespace alone::amasm {
    class Scanner {
    public:
        static std::string scan(std::fstream& file);
        static std::string scan(const std::string& input);
    };
}
