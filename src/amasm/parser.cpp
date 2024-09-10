#include "parser.hpp"

//std
#include <any>

namespace alone::amasm {
	std::vector<std::byte> Parser::parse(const token_array_t& tokens) {
		std::vector<std::byte> result;
		std::any current;

		//first iteration: detecting structures
		for(size_t i = 0; i != tokens.size();) {
			_generator[tokens[i].val](tokens, i);
		}

		return result;
	}

	std::unordered_map<std::string, inst_dispatcher_t> Parser::_generator = {
		std::make_pair("struct", [](const token_array_t& tokens, size_t& i) {
			info::data_type_t result(tokens[i + 1].val, 0);
			return result;
		}), std::make_pair("var", [](const token_array_t& tokens, size_t& i) {
			info::var_t result;

			result.name = tokens[i + 2].val;
			result.type = info::data_types[tokens[i + 4].val];
			result.offset = _read_address_offset(tokens, i).offset;

			return result;
		}),
	};

	info::address_offset_t Parser::_read_address_offset(const token_array_t& tokens, size_t& i) {
		info::address_offset_t result = { .begin = tokens[i + 2].val };
		switch(tokens[i + 4].type) {
		case info::binary:
			result.offset = std::stoi(tokens[i + 4].val.substr(2), nullptr, 2);
			break;
		case info::integer:
			result.offset = std::stoi(tokens[i + 4].val);
			break;
		case info::hexadecimal:
			result.offset = std::stoi(tokens[i + 4].val.substr(2), nullptr, 16);
			break;
		default:
			throw(std::exception("Wrong datatype inside address_offset"));
		}

		if(tokens[i + 3].type == info::plus)
			result.offset *= -1;

		i += 6;

		return result;
	}
}
