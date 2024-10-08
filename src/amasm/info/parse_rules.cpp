#include "parse_rules.hpp"

//std
#include <stack>

//alone
#include "general.hpp"
#include "error_codes.hpp"

//alone::amasm
#include "consts.hpp"

namespace alone::amasm {
	parse_rule_t::parse_rule_t(token_type t) :
		data(singular_token_rule_t { t }),
		type(parse_rule_type::singular_token) { //
	}
	parse_rule_t::parse_rule_t(const std::string& l) {
		switch (check_type(l)) {
		case literal_type::word:
			data = literal_rule_t { l };
			type = parse_rule_type::literal;
			break;
		case literal_type::binary:
			data = number_rule_t { std::stoull(l.substr(2), nullptr, 2) };
			type = parse_rule_type::number;
			break;
		case literal_type::decimal:
			data = number_rule_t { std::stoull(l) };
			type = parse_rule_type::number;
			break;
		case literal_type::hexadecimal:
			data = number_rule_t { std::stoull(l, nullptr, 16) };
			type = parse_rule_type::number;
			break;
		case literal_type::floating: {
			auto temp = std::stod(l);
			data = number_rule_t { reinterpret_cast<const size_t&>(temp) };
			type = parse_rule_type::number;
			break;
		}
		default:
			throw AMASM_PARSE_RULES_WRONG_LITERAL;
		}
	}
	parse_rule_t::parse_rule_t(size_t n) :
		data(number_rule_t { n }),
		type(parse_rule_type::number) { //
	}
	parse_rule_t::parse_rule_t(const std::initializer_list<std::string>& v) :
		type(parse_rule_type::sequence) {
		std::vector<parse_rule_ptr> on_place;
		on_place.reserve(v.size());
		for (const std::string& it : v) {
			if (auto lt = check_type(it); lt == literal_type::word || lt == literal_type::none)
				on_place.push_back(rules.at(it));
			else
				on_place.push_back(make_rule(it)); //should push number
		}
		data = sequence_rule_t { on_place };
	}
	parse_rule_t::parse_rule_t(const parse_rule_t::sequence_rule_t& v) :
		data(v),
		type(parse_rule_type::sequence) { //
	}
	parse_rule_t::parse_rule_t(parse_flag_type f) :
		data(parse_flag_rule_t { f }),
		type(parse_rule_type::flag) { //
	}

	const token_type& parse_rule_t::get_token() const {
		return std::get<singular_token_rule_t>(data);
	}
	const std::string& parse_rule_t::get_literal() const {
		return std::get<literal_rule_t>(data);
	}
	const size_t& parse_rule_t::get_number() const {
		return std::get<number_rule_t>(data);
	}
	const std::vector<parse_rule_ptr>& parse_rule_t::get_sequence() const {
		return std::get<sequence_rule_t>(data);
	}
	const parse_flag_type& parse_rule_t::get_flag() const {
		return std::get<parse_flag_rule_t>(data);
	}

	size_t parse_rule_t::get_length(const parse_rule_ptr& ptr) {
		size_t result = 0;
		std::stack<parse_rule_ptr> stack;

		stack.push(ptr);
		while (!stack.empty()) {
			auto top = stack.top();
			stack.pop();
			switch (top->type) {
			case parse_rule_type::sequence: {
				const auto& seq = top->get_sequence();
				for (const auto& it : seq | std::views::reverse)
					stack.push(it);
			}
				break;
			case parse_rule_type::singular_token:
			case parse_rule_type::literal:
			case parse_rule_type::number:
				result += 1;
				break;
			default:
				break;
			}
		}

		return result;
	}
}
