#include "info.hpp"

//alone
#include "general.hpp"

namespace alone::amasm {
	//token_t

	token_t::token_t() :
		type(token_type::none) {
	}
	token_t::token_t(token_type t, char c):
		type(t),
		literal(1, c) {
	}
	token_t::token_t(token_type t, std::string s):
		type(t),
		literal(std::move(s)) {
	}

	//parse_rule_t

	parse_rule_t::parse_rule_t(parse_rule_flag f) :
		content(f),
		type(parse_rule_type::flag) {
	}
	parse_rule_t::parse_rule_t(token_type t) :
		content(t),
		type(parse_rule_type::singular_token) {
	}
	parse_rule_t::parse_rule_t(std::string s):
		content(std::move(s)),
		type(parse_rule_type::literal) {
	}
	parse_rule_t::parse_rule_t(const std::initializer_list<std::string>& str_v) :
		type(parse_rule_type::sequence) {
		std::vector<parse_rule_ptr> temp;
		temp.reserve(str_v.size());
		for (const auto& s : str_v)
			temp.emplace_back(rules[s]);
		content = temp;
	}

	token_type parse_rule_t::get_token_type() const {
		return std::get<token_type>(content);
	}
	parse_rule_flag parse_rule_t::get_parse_rule_flag() const {
		return std::get<parse_rule_flag>(content);
	}
	const std::string& parse_rule_t::get_literal() const {
		return std::get<std::string>(content);
	}
	const std::vector<parse_rule_ptr> parse_rule_t::get_seq() const {
		return std::get<std::vector<parse_rule_ptr>>(content);
	}

	std::unordered_map<std::string, parse_rule_ptr> rules;
}
