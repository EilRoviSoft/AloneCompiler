//std
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

//alone::vm
#include "lexer.hpp"
#include "parser.hpp"
#include "scanner.hpp"
#include "vm/virtual_machine.hpp"

using namespace alone;

std::vector<std::function<int()>> tests = {
	/*[]() {
		int result = 0;
		std::vector<std::string> rules = {
			"identifier",
			"address_with_displacement"
		};
		std::string code = "push64 [%asx + 8]\n";
		if (!amasm::Parser::_match_rules(rules, amasm::Lexer::tokenize(code), 0))
			result = 1;
		return result;
	},*/
	[]() {
		vm::VirtualMachine vm;

		std::ifstream input_file("res/example.amasm");
		if (!input_file.is_open())
			return -1;
		auto scanned = amasm::Scanner::scan(input_file);
		input_file.close();

		auto tokenized = amasm::Lexer::tokenize(scanned);
		//auto parsed = amasm::Parser::parse(tokenized);

		return 0;
	}
};

int main() {
	amasm::Parser::init();

	int result = 0;
	for (size_t i = 0; i != tests.size(); ++i) {
		std::cout << "test " << i << ":\n";
		if ((result = tests[i]()))
			break;
		std::cout << '\n';
	}

	return result;
}
