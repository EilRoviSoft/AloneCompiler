#include "scanner.hpp"

//std
#include <ranges>
#include <vector>

namespace alone::amasm {
	std::string Scanner::scan(std::fstream& input) {
		std::string temp, buffer;

		while (!input.eof()) {
			std::getline(input, buffer, '\n');
			temp += buffer + '\n';
		}

		return scan(temp);
	}

	std::string Scanner::scan(const std::string& input) {
		std::string result;
		std::vector<std::pair<size_t, size_t>> ranges;
		size_t line_start_pos = 0;
		bool comment_flag = false;

		for (size_t i = 0; i != input.size(); ++i) {
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
		for (auto range : ranges)
			result += input.substr(range.first, range.second - range.first) + '\n';

		result.shrink_to_fit();
		return result;
	}
}
