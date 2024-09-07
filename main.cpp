//std
#include <fstream>

//alone::vm
#include "amasm/lexer.hpp"
#include "amasm/scanner.hpp"
#include "vm/virtual_machine.hpp"

using namespace alone;

int main() {
	vm::VirtualMachine vm;

	std::fstream file("example.amasm");
	if (!file.is_open())
		return -1;
	auto scanned = amasm::Scanner::scan(file);
	file.close();

	auto tokenized = amasm::Lexer::tokenize(scanned);

	return 0;
}
