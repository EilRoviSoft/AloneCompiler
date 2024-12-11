#include "extras.hpp"

//std
#include <ranges>

//alone::amasm::info
#include "consts.hpp"

namespace alone::amasm {
	data_type_ptr get_data_type(const std::string& str) {
		static const std::array<std::pair<std::string, std::string>, 14> map = {
			std::make_pair("8u", "uint8"),
			std::make_pair("16u", "uint16"),
			std::make_pair("32u", "uint32"),
			std::make_pair("64u", "uint64"),
			std::make_pair("8i", "int8"),
			std::make_pair("16i", "int16"),
			std::make_pair("32i", "int32"),
			std::make_pair("64i", "int64"),
			std::make_pair("32f", "float32"),
			std::make_pair("64f", "float64"),
			std::make_pair("8", "uint8"),
			std::make_pair("16", "uint16"),
			std::make_pair("32", "uint32"),
			std::make_pair("64", "uint64"),
		};

		const auto it = std::ranges::find_if(map, [&str](const auto& v) {
			return str.ends_with(v.first);
		});
		return data_types.at(it->second);
	}
}
