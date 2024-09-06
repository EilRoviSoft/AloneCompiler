//std
#include <fstream>

//alone::vm
#include "vm/virtual_machine.hpp"
#include "amasm/include.hpp"

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
