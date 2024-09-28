#include "info.hpp"

//std
#include <stdexcept>

//alone
#include "general.hpp"

namespace alone::amasm {
	//token_t

	token_t::token_t() :
		type(token_type::none) {
	}
	token_t::token_t(char c) :
		type((token_type) c),
		literal(1, c) {
	}
	token_t::token_t(std::string s) :
		literal(std::move(s)) {
		if (is_alpha(literal[0]) && check_on_property(literal.substr(1), &is_alpha_numeric)) {
			type = token_type::identifier;
		} else if (check_on_property(literal, &is_hexadecimal)) {
			type = token_type::number;
		} else
			throw std::runtime_error("lexer.cpp: Wrong input");
	}

	//parse_rule_t

	std::unordered_map<std::string, parse_rule_ptr> rules;

	parse_rule_t::parse_rule_t(parse_rule_flag flag) :
		c(flag),
		t(parse_rule_type::flag) {
	}
	parse_rule_t::parse_rule_t(token_type token) :
		c(token),
		t(parse_rule_type::singular_token) {
	}
	parse_rule_t::parse_rule_t(std::string str) :
		c(str),
		t(parse_rule_type::literal) {
	}
	parse_rule_t::parse_rule_t(const std::vector<std::string>& str_vec) :
		t(parse_rule_type::sequence) {
		std::vector<parse_rule_ptr> on_place;
		on_place.reserve(str_vec.size());
		for (const auto& it: str_vec)
			on_place.push_back(rules[it]);
		c = on_place;
	}

	const parse_rule_flag& parse_rule_t::get_flag() {
		return std::get<parse_rule_flag>(c);
	}
	const token_type& parse_rule_t::get_token() {
		return std::get<token_type>(c);
	}
	const std::string& parse_rule_t::get_literal() {
		return std::get<std::string>(c);
	}
	const std::vector<parse_rule_ptr>& parse_rule_t::get_sequence() {
		return std::get<std::vector<parse_rule_ptr>>(c);
	}
}
