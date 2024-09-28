#pragma once

//std
#include <fstream>
#include <istream>
#include <string>

namespace alone::amasm {
	class Scanner {
	public:
		static std::string scan(std::ifstream& file);
		static std::string scan(std::istream& stream, size_t size);
		static std::string scan(const std::string& input);
	};
}
