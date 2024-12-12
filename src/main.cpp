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
    int test_data_types() {
        for (const std::string& it : amasm::data_types | std::views::keys)
            std::cout << it << '\n';
        return 0;
    }
    int test_code_parsing() {
        std::fstream file("example.amasm");
        if (!file.is_open())
            return -1;
        auto text = amasm::Scanner::scan(file);
        file.close();

        auto tokens = amasm::Lexer::tokenize_code(text);
        auto code = amasm::Parser::parse(tokens);

        return 0;
    }
    int test_isa_collection() {
        auto sorted = std::ranges::to<std::vector>(lib::instructions_by_name | std::views::values);
        auto less = [](const lib::inst_ptr& lhs, const lib::inst_ptr& rhs) {
            return lhs->code < rhs->code;
        };

        std::ranges::sort(sorted, less);
        for (const auto& val : sorted)
            std::cout << val->name << '\t'
                    << val->code << '\t'
                    << val->max_args_count << '\t'
                    << val->bit_depth << '\n';
        std::cout << "isa size: " << sorted.size() << '\n';

        return 0;
    }
}

void init_tasks() {
    tests = {
        { false, testing_system::test_data_types },
        { false, testing_system::test_code_parsing },
        { true, testing_system::test_isa_collection }
    };
}

int main() {
    lib::init_isa();
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
