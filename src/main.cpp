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

std::vector<std::pair<bool, std::function<int()>>> tests = {
	std::make_pair(true, []() {
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
	}),
	std::make_pair(false, []() {
		vm::VirtualMachine vm;

		std::fstream input_file("res/example.amasm");
		if (!input_file.is_open())
			return -1;
		auto scanned = amasm::Scanner::scan(input_file);
		input_file.close();

		auto tokenized = amasm::Lexer::tokenize(scanned);
		//auto parsed = amasm::Parser::parse(tokenized);

		return 0;
	})
};

int main() {
	amasm::init_consts();

	auto f = [](const std::pair<bool, std::function<int()>>& item) {
		return item.first;
	};

	int result = 0, i = 0;
	for (const auto& it : tests | std::views::filter(f) | std::views::values) {
		result = it();
		std::cout << "test: " << i << " | " << (result ? "fail" : "success") << '\n';
		++i;
	}

	return result;
}
