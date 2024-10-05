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
	[]() {
		std::string guide = "struct_definition";
		std::fstream file("res/example.amasm");
		if (!file.is_open())
			return 1;
		std::string code = amasm::Scanner::scan(file);
		file.close();

		return (int) !amasm::Parser::match_rules(
			guide,
			amasm::Lexer::tokenize(code),
			0
		);
	},
	[]() {
		vm::VirtualMachine vm;

		std::fstream input_file("res/example.amasm");
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
	amasm::init_consts();

	int result = 0;
	for (size_t i = 0; i != tests.size(); ++i) {
		std::cout << "test " << i << ":\n";
		if ((result = tests[i]()))
			break;
		std::cout << '\n';
	}

	return result;
}
