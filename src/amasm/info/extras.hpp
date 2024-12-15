#pragma once

//std
#include <string>

//alone
#include "data_types.hpp"
#include "general.hpp"

namespace alone::amasm {
    struct argument_t {
        //indirect -> name, immediate -> value, indirect_with_displacement -> [name + value]
        lib::argument_type type;
        std::string name;
        ptrdiff_t value;
    };
    //using argument_t = std::variant<size_t, std::string, std::tuple<std::string, ptrdiff_t>>;

    data_type_ptr get_data_type(const std::string& str);
}
