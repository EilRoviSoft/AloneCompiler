//std
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

//alone
#include "instructions.hpp"

//alone::amasm
#include "amasm/lexer.hpp"
#include "amasm/parser.hpp"
#include "amasm/scanner.hpp"

//alone::vm
#include "vm/virtual_machine.hpp"

using namespace alone;

std::vector<std::tuple<bool, std::function<int()>>> tests;

namespace testing_system {
    lib::Bytecode generate_bytecode(const std::string& filename) {
        std::fstream file(filename, std::fstream::in);
        if (!file.is_open())
            return {};
        const auto text = amasm::Scanner::scan(file);
        file.close();

        const auto tokens = amasm::Lexer::tokenize_code(text);

        return amasm::Parser::parse(tokens);
    }

    int test_data_types() {
        std::fstream file("datatypes.txt", std::ofstream::out | std::ofstream::trunc);
        if (!file.is_open())
            return -1;
        for (const auto& it : amasm::data_types | std::views::values) {
            file << it->name << "\n\tsize: " << it->size << "\n";

            if (!it->poles.empty()) {
                file << "\tpoles: " << it->poles.size() << "\n";
                for (const auto& pole : it->poles)
                    file << "\t\t" << pole.offset << '\t' << pole.name << '\n';
            }
            file << std::endl;
        }
        file.close();
        return 0;
    }
    int test_isa_collection() {
        auto sorted = std::ranges::to<std::vector>(lib::instructions_by_name | std::views::values);
        auto less = [](const lib::inst_ptr& lhs, const lib::inst_ptr& rhs) {
            return lhs->code < rhs->code;
        };

        std::ranges::sort(sorted, less);

        std::fstream file("isa.txt", std::ofstream::out | std::ofstream::trunc);
        if (!file.is_open())
            return -1;
        for (const auto& val : sorted) {
            file << val->name << (val->name.size() <= 3 ? " \t" : "\t")
                    << val->code << '\t'
                    << val->min_args_count << ' '
                    << val->max_args_count << ' '
                    << val->bit_depth << '\n';
        }
        file << "isa size: " << sorted.size() << '\n';
        file.close();

        return 0;
    }
    int test_code() {
        const auto code = generate_bytecode("example.amasm");

        std::fstream file("output.txt", std::ofstream::out | std::ofstream::trunc);
        if (!file.is_open())
            return -1;
        auto temp = code.as_vector();
        file.write(reinterpret_cast<const char*>(temp.data()), temp.size());
        file.close();

        vm::VirtualMachine vm;
        vm.exec(code);

        return 0;
    }
}

void init_tasks() {
    tests = {
        { false, testing_system::test_data_types },
        { false, testing_system::test_isa_collection },
        { true, testing_system::test_code }
    };
}

//TODO: separate virtual_machine from parser

int main() {
    lib::init_isa();
    amasm::init_consts();
    init_tasks();

    auto check_condition = [](const std::pair<bool, std::function<int()>>& item) {
        return item.first;
    };

    int result = 0, i = 0;
    for (const auto& it : tests | std::views::filter(check_condition) | std::views::values) {
        result = it();
        std::cout << "test: " << i << " | " << (result ? "fail" : "success") << '\n';
        ++i;
    }

    return result;
}
