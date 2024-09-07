#include "lexer.hpp"

//alone::amasm::util
#include "amasm/info.hpp"
#include "amasm/util.hpp"

namespace alone::amasm {
	info::token_array_t Lexer::tokenize(const std::string& code) {
		info::token_array_t result;

		if(!_is_init)
			_init();

		for(size_t i = 0; i != code.size();) {
			if (util::is_whitespace(code[i])) {
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
	std::map<char, info::token_dispatcher_t> Lexer::_token_machine;
	const std::string Lexer::_alpha = '_' + util::gen_str('a', 'z') + util::gen_str('A', 'Z');
	const std::string Lexer::_numeric = util::gen_str('0', '9');
	const std::string Lexer::_alpha_numeric = _alpha + _numeric;
	bool Lexer::_is_init = false;

	void Lexer::_init() {
		util::sync_identifiers();
		for(size_t i = 0; i != _singular_characters.size(); ++i)
			_token_machine.emplace(_gen_single_char_token_pair(_singular_characters[i], (info::token_type) i));
		for(auto c : _alpha)
			_token_machine.emplace(_gen_identifier(c));
		for(auto c : _numeric)
			_token_machine.emplace(_gen_number(c));
	}

	std::string Lexer::_get_until(const std::string& s, size_t& i, const std::function<bool(char)>& f) {
		std::string result;
		do {
			result += s[i];
			i++;
		} while(i != s.size() && f(s[i]));
		return result;
	}

	std::pair<char, info::token_dispatcher_t> Lexer::_gen_single_char_token_pair(char c, info::token_type t) {
		return  { c, info::token_dispatcher_t([c, t](const std::string& s, size_t& i) {
			++i;
			return info::token_t(c, t);
		}) };
	}

	std::pair<char, info::token_dispatcher_t> Lexer::_gen_identifier(char c) {
		return { c, info::token_dispatcher_t([](const std::string& s, size_t& i) {
			info::token_t result;

			result.v += _get_until(s, i, &util::is_alpha_numeric);
			auto t = info::identifiers.find(result.v);
			result.t = t != info::identifiers.end() ? t->second : info::none;

			return result;
		}) };
	}

	std::pair<char, info::token_dispatcher_t> Lexer::_gen_number(char c) {
		return { c, info::token_dispatcher_t([](const std::string& s, size_t& i) {
			info::token_t result;

			if (s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'b')) { //it's hexadecimal or binary
				std::function<bool(char)> check_type;
				switch(s[i + 1]) {
				case 'x':
					result = { "0x", info::hexadecimal };
					check_type = &util::is_hexadecimal;
					break;
				case 'b':
					result = { "0b", info::binary };
					check_type = &util::is_binary;
					break;
				default: 
					throw(std::exception("Idk how you did this error..."));
				}
				i += 2;
				result.v += _get_until(s, i, check_type);
			} else { //it's integer or floating
				result.v += _get_until(s, i, &util::is_numeric);
				if (s[i] == '.') {
					result.t = info::floating;
					result.v += _get_until(s, i, &util::is_numeric);
				} else
					result.t = info::integer;
			}

			return result;
		}) };
	}
}
