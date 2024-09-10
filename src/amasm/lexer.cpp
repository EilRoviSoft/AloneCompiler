#include "lexer.hpp"

//alone
#include "general.hpp"

//alone::amasm::util
#include "amasm/info.hpp"
#include "amasm/util.hpp"

namespace alone::amasm {
	std::vector<info::token_t> Lexer::tokenize(const std::string& code) {
		std::vector<info::token_t> result;

		if(!_is_init)
			_init();

		for(size_t i = 0; i != code.size();) {
			if (is_whitespace(code[i])) {
				++i;
				continue;
			}
			result.emplace_back(_token_generator[code[i]](code, i));
		}
		
		result.shrink_to_fit();
		return result;
	}

	const std::array<char, 17> Lexer::_singular_characters = {
		'(', ')', '[', ']', '{', '}',
		'.', ',', ':', '@', '$', '%', '\"',
		'+', '-', '*', '/'
	};
	std::unordered_map<char, info::token_dispatcher_t> Lexer::_token_generator;
	const std::string Lexer::_alpha = '_' + gen_str('a', 'z') + gen_str('A', 'Z');
	const std::string Lexer::_numeric = gen_str('0', '9');
	const std::string Lexer::_alpha_numeric = _alpha + _numeric;
	bool Lexer::_is_init = false;

	void Lexer::_init() {
		util::sync_identifiers();
		for(size_t i = 0; i != _singular_characters.size(); ++i)
			_token_generator.emplace(_gen_single_char_token_pair(_singular_characters[i], (info::token_type) i));
		for(auto c : _alpha)
			_token_generator.emplace(_gen_identifier(c));
		for(auto c : _numeric)
			_token_generator.emplace(_gen_number(c));
		_token_generator.emplace('\"', [](const std::string& s, size_t& i) {
			return info::token_t(_get_until(s, i, [](char c) { return c == '\"'; }), info::string);
		});
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

			result.val += _get_until(s, i, &is_alpha_numeric);
			auto t = info::identifiers.find(result.val);
			result.type = t != info::identifiers.end() ? t->second : info::identifier;

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
					check_type = &is_hexadecimal;
					break;
				case 'b':
					result = { "0b", info::binary };
					check_type = &is_binary;
					break;
				default: 
					throw(std::exception("Idk how you did this error..."));
				}
				i += 2;
				result.val += _get_until(s, i, check_type);
			} else { //it's integer or floating
				result.val += _get_until(s, i, &is_numeric);
				if (s[i] == '.') {
					result.type = info::floating;
					result.val += _get_until(s, i, &is_numeric);
				} else
					result.type = info::integer;
			}

			return result;
		}) };
	}
}
