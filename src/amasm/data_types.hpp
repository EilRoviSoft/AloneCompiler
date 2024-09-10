#pragma once

//std
#include <optional>
#include <string>
#include <vector>
#include <memory>

//alone
#include "general.hpp"

namespace alone::amasm::info {
	struct var_t {
		size_t offset, scope_level;
		std::string name;
		std::shared_ptr<struct data_type_t> type;
	};
	using poles_t = std::optional<std::vector<var_t>>;

	struct data_type_t {
		std::string literal;
		size_t size;
		poles_t poles;

		data_type_t(std::string l, size_t s, poles_t p = std::nullopt) :
			literal(std::move(l)),
			size(s),
			poles(std::move(p)) {
		}
	};
}
