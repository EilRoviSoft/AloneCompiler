#include "include.hpp"

namespace alone::amasm {
	bool is_numeric_alpha(char c) {
		return c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
	}

	//token_t

	token_t::token_t(char ch, info::token_type type) : v(std::string(1, ch)), t(type) {}
	token_t::token_t(const std::string& str = "", info::token_type type = info::none) : v(str), t(type) {}

	//Scanner

	std::string Scanner::scan(std::istream& input) {
		std::string temp, buffer;

		do {
			std::getline(input, buffer);
			temp += buffer + '\n';
		} while (!buffer.empty());

		return scan(temp);
	}

	std::string Scanner::scan(const std::string& input) {
		std::string result;
		std::vector<std::pair<size_t, size_t>> ranges;
		size_t line_start_pos = 0;
		bool comment_flag = false;

		for (size_t i = 0; i != input.size(); ++i) {
			if (input[i] == '#') {
				comment_flag = true;
				size_t j;
				for (j = i; input[j - 1] == '\t' || input[j - 1] == ' '; --j);
				ranges.emplace_back(line_start_pos, j);
			} else if (input[i] == '\n') {
				if (!comment_flag && line_start_pos != i)
					ranges.emplace_back(line_start_pos, i);
				line_start_pos = i + 1;
				comment_flag = false;
			}
		}

		result.reserve(ranges.size());
		for (auto range : ranges)
			result += input.substr(range.first, range.second - range.first) + '\n';

		return result;
	}

	//Lexer

	token_array_t Lexer::tokenize(const std::string& code) {
		token_array_t result;

		if (!_is_init)
			_init();

		for (size_t i = 0; i != code.size();)
			result.emplace_back(_token_machine[code[i]](code, i));

		return result;
	}

	const std::array<char, 14> Lexer::_singular_characters = {
		'(', ')', '[', ']',
		'.', ',', ':', '@', '%', '\"',
		'+', '-', '*', '/'
	};
	std::unordered_map <char, token_dispatcher_t> Lexer::_token_machine;
	bool Lexer::_is_init = false;

	void Lexer::_init() {
		for (size_t i = 0; i != _singular_characters.size(); ++i)
			_token_machine.emplace(_gen_single_char_token_pair(_singular_characters[i], (info::token_type) i));
		_token_machine.emplace();
	}

	std::pair<char, token_dispatcher_t> Lexer::_gen_single_char_token_pair(char c, info::token_type t) {
		return  { c, token_dispatcher_t([c, t](const std::string& s, size_t& i) {
			return token_t(c, t);
		}) };
	}

	std::pair<char, token_dispatcher_t> Lexer::_gen_identifier(char c) {
		return { c, token_dispatcher_t([](const std::string& s, size_t& i) {
			token_t result;

			while (i != s.size()) {

			}

			return result;
		}) };
	}
}
