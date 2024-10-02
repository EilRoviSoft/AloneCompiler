#include "info.hpp"

//std
#include <stdexcept>
#include <ranges>
#include <stack>

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
		switch (check_type(literal)) {
		case literal_type::word: {
			auto temp = keyword_tokens.find(s);
			type = temp == keyword_tokens.end() ? token_type::identifier : temp->second;
			break;
		}
		case literal_type::binary:
		case literal_type::decimal:
		case literal_type::hexadecimal:
		case literal_type::floating:
			type = token_type::number;
			break;
		default:
			throw std::runtime_error("lexer.cpp: Wrong input");
		}
	}

	//parse_rule_t

	parse_rule_t::parse_rule_t(parse_rule_flag flag) :
		data(flag),
		type(parse_rule_type::flag) {
	}
	parse_rule_t::parse_rule_t(token_type token) :
		data(token),
		type(parse_rule_type::singular_token) {
	}
	parse_rule_t::parse_rule_t(const std::string& str) {
		switch (check_type(str)) {
		case literal_type::word:
			data = str;
			type = parse_rule_type::literal;
			break;
		case literal_type::binary:
			data = std::stoull(str.substr(2), nullptr, 2);
			type = parse_rule_type::number;
			break;
		case literal_type::decimal:
			data = std::stoull(str);
			type = parse_rule_type::number;
			break;
		case literal_type::hexadecimal:
			data = std::stoull(str, nullptr, 16);
			type = parse_rule_type::number;
			break;
		case literal_type::floating: {
			auto temp = std::stod(str);
			data = reinterpret_cast<const size_t&>(temp);
			type = parse_rule_type::number;
			break;
		}
		default:
			throw std::runtime_error("info.cpp: Wrong literal");
		}
	}
	parse_rule_t::parse_rule_t(size_t num) :
		data(num),
		type(parse_rule_type::number) {
	}
	parse_rule_t::parse_rule_t(const std::vector<std::string>& str_vec) :
		type(parse_rule_type::sequence) {
		std::vector<parse_rule_ptr> on_place;
		on_place.reserve(str_vec.size());
		for (const auto& it : str_vec) {
			auto lt = check_type(it);
			if (lt == literal_type::word || lt == literal_type::none)
				on_place.push_back(rules_collection.at(it));
			else
				on_place.push_back(make_rule(it)); //should push number
		}
		data = on_place;
	}

	const parse_rule_flag& parse_rule_t::get_flag() {
		return std::get<parse_rule_flag>(data);
	}
	const token_type& parse_rule_t::get_token() {
		return std::get<token_type>(data);
	}
	const std::string& parse_rule_t::get_literal() {
		return std::get<std::string>(data);
	}
	const size_t& parse_rule_t::get_number() {
		return std::get<size_t>(data);
	}
	const std::vector<parse_rule_ptr>& parse_rule_t::get_sequence() {
		return std::get<std::vector<parse_rule_ptr>>(data);
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

	//consts

	std::unordered_set<char> singular_tokens;
	std::unordered_map<std::string, token_type> keyword_tokens;
	std::unordered_map<std::string, parse_rule_ptr> rules_collection;

	void init_consts() {
		singular_tokens = {
			'(', ')', '[', ']', '{', '}',
			'.', ',', ':', ';',
			'@', '$', '%', '\"',
			'+', '-', '*', '/'
		};
		keyword_tokens = {
			{ "section", token_type::section_keyword },
			{ "label", token_type::label_keyword },
			{ "func", token_type::function_keyword },
			{ "struct", token_type::struct_keyword }
		};

		rules_collection.emplace("?", make_rule(parse_rule_flag::optional));
		rules_collection.emplace("|", make_rule(parse_rule_flag::variant));
		rules_collection.emplace("(", make_rule(token_type::lparen));
		rules_collection.emplace(")", make_rule(token_type::rparen));
		rules_collection.emplace("[", make_rule(token_type::lbracket));
		rules_collection.emplace("]", make_rule(token_type::rbracket));
		rules_collection.emplace("{", make_rule(token_type::lbrace));
		rules_collection.emplace("}", make_rule(token_type::rbrace));
		rules_collection.emplace(".", make_rule(token_type::dot));
		rules_collection.emplace(",", make_rule(token_type::comma));
		rules_collection.emplace(":", make_rule(token_type::colon));
		rules_collection.emplace(";", make_rule(token_type::semicolon));
		rules_collection.emplace("@", make_rule(token_type::at));
		rules_collection.emplace("$", make_rule(token_type::dollar));
		rules_collection.emplace("%", make_rule(token_type::percent));
		rules_collection.emplace("\"", make_rule(token_type::quote));
		rules_collection.emplace("+", make_rule(token_type::plus));
		rules_collection.emplace("-", make_rule(token_type::minus));
		rules_collection.emplace("*", make_rule(token_type::star));
		rules_collection.emplace("/", make_rule(token_type::slash));
		rules_collection.emplace("identifier", make_rule(token_type::identifier));
		rules_collection.emplace("number", make_rule(token_type::number));
		rules_collection.emplace("data_type", make_rule(token_type::data_type));
		rules_collection.emplace("section_keyword", make_rule(token_type::section_keyword));
		rules_collection.emplace("label_keyword", make_rule(token_type::label_keyword));
		rules_collection.emplace("func_keyword", make_rule(token_type::function_keyword));
		rules_collection.emplace("struct_keyword", make_rule(token_type::struct_keyword));
		rules_collection.emplace(
			"address_with_displacement",
			make_rule(std::initializer_list<std::string> { "[", "%", "identifier", "|", "2", "+", "-", "]" })
		);
		rules_collection.emplace(
			"alloc_array_size",
			make_rule(std::initializer_list<std::string> { "[", "data_type", "*", "number", "]" })
		);
	}
}
