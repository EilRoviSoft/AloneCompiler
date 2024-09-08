#pragma once

//std
#include <optional>
#include <string>
#include <vector>
#include <memory>

//alone
#include "general.hpp"

namespace alone::amasm::info {
	using poles_t = std::optional<std::vector<std::shared_ptr<struct data_type_t>>>;

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

	struct data_type_hasher {
		size_t operator()(const std::shared_ptr<data_type_t>& type) const {
			return string_hasher(type->literal);
		}
	};
}
