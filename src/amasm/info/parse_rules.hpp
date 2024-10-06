#pragma once

//std
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <concepts>

//alone::amasm
#include "info.hpp"

namespace alone::amasm {
	struct parse_rule_t;
	using parse_rule_ptr = std::shared_ptr<parse_rule_t>;

	struct parse_rule_t {
		using singular_token_rule_t = token_type;
		using literal_rule_t = std::string;
		using number_rule_t = size_t;
		using sequence_rule_t = std::vector<parse_rule_ptr>;
		using parse_flag_rule_t = parse_flag_type;

		using parse_rule_variant = std::variant<singular_token_rule_t, literal_rule_t, number_rule_t, sequence_rule_t,
			parse_flag_rule_t>;

		parse_rule_variant data;
		parse_rule_type type;

		explicit parse_rule_t(token_type t);
		explicit parse_rule_t(const std::string& l);
		explicit parse_rule_t(size_t n);
		parse_rule_t(const std::initializer_list<std::string>& v);
		explicit parse_rule_t(const sequence_rule_t& v);
		//f = [optional, variant, skip_until_next]
		explicit parse_rule_t(parse_flag_type f);

		const token_type& get_token() const;
		const std::string& get_literal() const;
		const size_t& get_number() const;
		const std::vector<parse_rule_ptr>& get_sequence() const;
		const parse_flag_type& get_flag() const;

		static size_t get_length(const parse_rule_ptr& ptr);
	};

	template<typename T>
	requires std::constructible_from<parse_rule_t, T>
	inline parse_rule_ptr make_rule(const T& arg) {
		return std::make_shared<parse_rule_t>(arg);
	}
	inline parse_rule_ptr make_rule(std::initializer_list<std::string> list) {
		return std::make_shared<parse_rule_t>(list);
	}
}
