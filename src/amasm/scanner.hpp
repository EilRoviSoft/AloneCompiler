#pragma once

//std
#include <istream>
#include <string>

namespace alone::amasm {
	class Scanner {
	public:
		static std::string scan(std::istream& file);
		static std::string scan(const std::string& input);
	};
}
