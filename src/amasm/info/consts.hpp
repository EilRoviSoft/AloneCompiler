#pragma once

//std
#include <string>
#include <unordered_map>
#include <unordered_set>

//alone::amasm::info
#include "data_types.hpp"
#include "enums.hpp"
#include "extras.hpp"

namespace alone::amasm {
    extern std::unordered_set<char> singular_tokens;
    extern std::unordered_map<std::string, token_type> defined_tokens;
    extern std::unordered_map<std::string, data_type_ptr> data_types;
    extern std::unordered_map<std::string, std::vector<token_type>> rules;
    extern std::unordered_map<std::string, variable_t> predefined_vars;

    void add_data_type(const data_type_ptr& ptr);

    void init_consts();
}
