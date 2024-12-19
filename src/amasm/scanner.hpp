#pragma once

//std
#include <fstream>
#include <string>

//alone::amasm::info
#include "amasm/info/context.hpp"

namespace alone::amasm {
    class Scanner {
    public:
        explicit Scanner(const AmasmContext& amasm_context);

        std::string scan(std::fstream& file) const;
        std::string scan(const std::string& input) const;

    private:
        const AmasmContext& _amasm_context;
    };
}
