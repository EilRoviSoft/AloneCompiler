#include "info.hpp"

//alone
#include "general.hpp"

namespace alone::amasm {
	//token_t

	token_t::token_t() :
		type(token_type::none) {
	}
	token_t::token_t(token_type t, char c) :
		type(t),
		literal(1, c) {
	}
	token_t::token_t(token_type t, std::string s) :
		type(t),
		literal(std::move(s)) {
	}

	//parse_rule_t

	std::unordered_map<std::string, parse_rule_ptr> rules;

	parse_rule_t::parse_rule_t(parse_rule_flag flag) :
		c(flag),
		t(parse_rule_type::flag) {
		if (flag != parse_rule_flag::optional)
			throw std::runtime_error("info.hpp: Wrong parse_rule_t flag.");
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

	template<typename T>
	const T& parse_rule_t::get() {
		return std::get<const T&>(c);
	}

}
