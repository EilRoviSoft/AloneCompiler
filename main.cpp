//std
#include <iostream>

//eil::vm
#include <virtual_machine.hpp>

using namespace eil;
using enum registers;

int main() {
	virtual_machine vm;
	int& val = vm.ctx().get_var <int>(rsx);
	val = std::pow(2, 17) + 1004;
}