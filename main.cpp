//std
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

//alone::vm
#include "amasm/lexer.hpp"
#include "amasm/parser.hpp"
#include "amasm/scanner.hpp"
#include "vm/virtual_machine.hpp"

using namespace alone;

std::vector<std::function<int()>> tests = {
	[]() {
		vm::VirtualMachine vm;

		std::ifstream input_file("example.amasm");
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
	int result = 0;
	for (size_t i = 0; i != tests.size(); ++i) {
		std::cout << "test " << i << ":\n";
		if ((result = tests[i]()) != 0)
			break;
		std::cout << '\n';
	}

	return result;
}
