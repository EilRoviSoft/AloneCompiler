//std
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

//alone::amasm
#include "amasm/lexer.hpp"
#include "amasm/parser.hpp"
#include "amasm/scanner.hpp"

//alone::vm
#include "vm/virtual_machine.hpp"

using namespace alone;

std::vector<std::pair<bool, std::function<int()>>> tests = {
	std::make_pair(false, []() {
		std::string guide = "address_with_displacement";
		std::fstream file("res/match_test.amasm");
		if (!file.is_open())
			return -1;
		auto text = amasm::Scanner::scan(file);
		file.close();

		return (int) !amasm::Parser::match_rules(
			guide,
			amasm::Lexer::tokenize_code(text),
			0
		);
	}),
	std::make_pair(true, []() {
		std::fstream file("res/example.amasm");
		if (!file.is_open())
			return -1;
		auto text = amasm::Scanner::scan(file);
		file.close();

		auto tokens = amasm::Lexer::tokenize_code(text);
		auto code = amasm::Parser::parse(tokens);

		return 0;
	}),
	std::make_pair(true, []() {
		for (const auto& it : amasm::data_types | std::views::keys)
			std::cout << it << '\n';

		return 0;
	}),
	std::make_pair(false, []() {
		vm::VirtualMachine vm;

		std::fstream input_file("res/example.amasm");
		if (!input_file.is_open())
			return -1;
		auto scanned = amasm::Scanner::scan(input_file);
		input_file.close();

		auto tokenized = amasm::Lexer::tokenize_code(scanned);
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
