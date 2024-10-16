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

std::vector<std::tuple<bool, std::function<int()>>> tests;

void init_tasks() {
	tests.emplace_back(true, []() {
		for (const std::string& it : amasm::data_types | std::views::keys)
			std::cout << it << '\n';

		return 0;
	});
	tests.emplace_back(true, []() {
		std::ofstream output("test.txt");
		output << "hello world\n";
		output.close();

		return 0;
	});
	tests.emplace_back(true, []() {
		std::fstream file("example.amasm");
		if (!file.is_open())
			return -1;
		auto text = amasm::Scanner::scan(file);
		file.close();

		auto tokens = amasm::Lexer::tokenize_code(text);
		auto code = amasm::Parser::parse(tokens);

		return 0;
	});
}

int main() {
	amasm::init_consts();
	init_tasks();

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
