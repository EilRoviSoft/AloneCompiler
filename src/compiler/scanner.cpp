#include "compiler/scanner.hpp"

//std
#include <ranges>
#include <vector>

namespace amasm::compiler {
    std::string Scanner::scan_from_file(std::fstream& file) {
        std::string temp, buffer;

        while (!file.eof()) {
            std::getline(file, buffer, '\n');
            temp += buffer + '\n';
        }

        return format_string(temp);
    }
    std::string Scanner::scan_from_file(const std::string& filename) {
        std::string temp, buffer;

        std::fstream file(filename);
        while (!file.eof()) {
            std::getline(file, buffer, '\n');
            temp += buffer + '\n';
        }
        file.close();

        return format_string(temp);
    }

    std::string Scanner::format_string(const std::string& input) {
        std::string result;
        std::vector<std::pair<size_t, size_t>> ranges;
        size_t line_start_pos = 0;
        bool comment_flag = false;

        for (size_t i = 0; i < input.size(); ++i) {
            if (input[i] == '#') {
                comment_flag = true;
                size_t j;
                for (j = i; input[j - 1] == '\t' || input[j - 1] == ' '; --j);
                ranges.emplace_back(line_start_pos, j);
            } else if (input[i] == '\n') {
                if (!comment_flag && line_start_pos != i)
                    ranges.emplace_back(line_start_pos, i);
                line_start_pos = i + 1;
                comment_flag = false;
            }
        }

        result.reserve(ranges.size());
        for (auto [begin, end] : ranges)
            result += input.substr(begin, end - begin) + '\n';
        result.shrink_to_fit();

        return result;
    }
}
