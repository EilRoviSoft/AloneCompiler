#include "library/logger.hpp"

//std
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

//frozen
#include "frozen/string.h"

namespace amasm::lib::logger {
    static constexpr std::array<frozen::string, Logger::Max> _filenames = {
        "", "error.log", ""
    };

    std::array<std::ostream*, Logger::Max> channels;
    std::vector<std::shared_ptr<std::ostream>> managed;
}

namespace amasm::lib {
    void Logger::init() {
        using namespace std::string_literals;

        for (size_t i = 0; i < Max; i++) {
            if (logger::_filenames[i] != "") {
                logger::managed.emplace_back(std::make_shared<std::fstream>(logger::_filenames[i].data()));
                logger::channels[i] = logger::managed.back().get();
            } else
                logger::channels[i] = &std::cout;
        }
    }

    std::ostream& Logger::channel(Channel id) {
        return *logger::channels[id];
    }
}
