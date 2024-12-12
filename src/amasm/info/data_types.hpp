#pragma once

//std
#include <concepts>
#include <memory>
#include <string>
#include <vector>

namespace alone::amasm {
    struct data_type_t;
    using data_type_ptr = std::shared_ptr<data_type_t>;

    struct pole_t {
        std::string name;
        data_type_ptr type;
        size_t offset;

        pole_t(std::string n, data_type_ptr t, const size_t& o = 0);
    };

    struct data_type_t {
        std::string name;
        size_t size;
        std::vector<pole_t> poles;

        explicit data_type_t(std::string n, const size_t& s);
        explicit data_type_t(std::string n, std::vector<pole_t> v);

        void add_pole(const std::string& name, const data_type_ptr& t);
    };

    struct variable_t {
        std::string name;
        data_type_ptr type;
    };

    template<typename T>
        requires std::constructible_from<data_type_t, std::string, T>
    data_type_ptr make_data_type(std::string n, T id_arg) {
        return std::make_shared<data_type_t>(n, id_arg);
    }
}
