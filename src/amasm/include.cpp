#include "include.hpp"

#include "util.hpp"

namespace alone::amasm {
	constexpr bool is_alpha(char c) {
		return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
	}
	constexpr bool is_binary(char c) {
		return c == '0' || c == '1';
	}
	constexpr bool is_numeric(char c) {
		return c >= '0' && c <= '9';
	}
	constexpr bool is_hexadecimal(char c) {
		return is_numeric(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
	}
	constexpr bool is_alpha_numeric(char c) {
		return is_alpha(c) || is_numeric(c);
	}
	constexpr bool is_whitespace(char c) {
		return c == ' ' || c == '\n' || c == '\t';
	}

	std::string gen_str(char a, char b) {
		std::string result;

		result.resize(b - a + 1);
		for(char i = a; i <= b; ++i)
			result[i - a] = i;

		return result;
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
		} while(!buffer.empty());

		return scan(temp);
	}

	std::string Scanner::scan(const std::string& input) {
		std::string result;
		std::vector<std::pair<size_t, size_t>> ranges;
		size_t line_start_pos = 0;
		bool comment_flag = false;

		for(size_t i = 0; i != input.size(); ++i) {
			if(input[i] == '#') {
				comment_flag = true;
				size_t j;
				for(j = i; input[j - 1] == '\t' || input[j - 1] == ' '; --j);
				ranges.emplace_back(line_start_pos, j);
			} else if(input[i] == '\n') {
				if(!comment_flag && line_start_pos != i)
					ranges.emplace_back(line_start_pos, i);
				line_start_pos = i + 1;
				comment_flag = false;
			}
		}

		result.reserve(ranges.size());
		for(auto range : ranges)
			result += input.substr(range.first, range.second - range.first) + '\n';

		return result;
	}

	//Lexer

	token_array_t Lexer::tokenize(const std::string& code) {
		token_array_t result;

		if(!_is_init)
			_init();

		for(size_t i = 0; i != code.size();) {
			if (is_whitespace(code[i])) {
				++i;
				continue;
			}
			result.emplace_back(_token_machine[code[i]](code, i));
		}

		return result;
	}

	const std::array<char, 14> Lexer::_singular_characters = {
		'(', ')', '[', ']',
		'.', ',', ':', '@', '%', '\"',
		'+', '-', '*', '/'
	};
	std::map<char, token_dispatcher_t> Lexer::_token_machine;
	const std::string Lexer::_alpha = '_' + gen_str('a', 'z') + gen_str('A', 'Z');
	const std::string Lexer::_numeric = gen_str('0', '9');
	const std::string Lexer::_alpha_numeric = _alpha + _numeric;
	bool Lexer::_is_init = false;

	void Lexer::_init() {
		for(size_t i = 0; i != _singular_characters.size(); ++i)
			_token_machine.emplace(_gen_single_char_token_pair(_singular_characters[i], (info::token_type) i));
		for(auto c : _alpha)
			_token_machine.emplace(_gen_identifier(c));
	}

	std::pair<char, token_dispatcher_t> Lexer::_gen_single_char_token_pair(char c, info::token_type t) {
		return  { c, token_dispatcher_t([c, t](const std::string& s, size_t& i) {
			++i;
			return token_t(c, t);
		}) };
	}

	std::pair<char, token_dispatcher_t> Lexer::_gen_identifier(char c) {
		return { c, token_dispatcher_t([](const std::string& s, size_t& i) {
			token_t result;

			do {
				result.v += s[i++];
			} while(i != s.size() && is_alpha_numeric(s[i]));

			auto t = util::identifiers.find(result.v);
			result.t = t != util::identifiers.end() ? t->second : info::none;

			return result;
		}) };
	}

	std::pair<char, token_dispatcher_t> Lexer::_gen_number(char c) {
		return { c, token_dispatcher_t([](const std::string& s, size_t& i) {
			token_t result;

			if (s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'b')) { //it's hexadecimal or binary
				std::function<bool(char)> check_type;
				switch(s[i + 1]) {
				case 'x':
					result = { "0x", info::hexadecimal };
					check_type = &is_hexadecimal;
					break;
				case 'b':
					result = { "0b", info::binary };
					check_type = &is_binary;
					break;
				}
				i += 2;
				do {
					result.v += s[i++];
				} while(i != s.size() && check_type(s[i]));
			} else { //it's integer or floating
				
			}

			return result;
		}) };
	}
}
