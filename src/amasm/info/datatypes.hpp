#pragma once

//std
#include <concepts>
#include <memory>
#include <string>
#include <vector>

namespace alone::amasm {
    using datatype_ptr = std::shared_ptr<struct datatype_t>;

    struct pole_t {
        std::string name;
        datatype_ptr type;
        size_t offset;

        pole_t(std::string name, datatype_ptr type, const size_t& offset) :
            name(std::move(name)),
            type(std::move(type)),
            offset(offset) {
        }
    };

    struct datatype_t {
        std::string name;
        size_t size;
        std::vector<pole_t> poles;

        datatype_t(std::string name, const size_t& size) :
            name(std::move(name)),
            size(size) {
        }
        datatype_t(std::string name, std::vector<pole_t> vec) :
            name(std::move(name)),
            size(0),
            poles(std::move(vec)) {
            for (const auto& it : poles)
                size += it.type->size;
            for (size_t i = 0; i != poles.size() - 1; i++)
                poles[i + i].offset = poles[i].offset + poles[i].type->size;
        }
    };

    template<typename T>
        requires std::constructible_from<datatype_t, std::string, T>
    datatype_ptr make_datatype(std::string name, T arg) {
        return std::make_shared<datatype_t>(std::move(name), std::move(arg));
    }
}
