#pragma once

//std
#include <concepts>
#include <memory>
#include <string>
#include <vector>

namespace amasm::compiler {
    using datatype_ptr = std::shared_ptr<struct datatype>;

    struct pole {
        std::string name;
        datatype_ptr type;
        ptrdiff_t offset;

        pole(std::string name, datatype_ptr type, ptrdiff_t offset) :
            name(std::move(name)),
            type(std::move(type)),
            offset(offset) {
        }
    };

    struct datatype {
        std::string name;
        size_t size;
        std::vector<pole> poles;

        datatype(std::string name, size_t size) :
            name(std::move(name)),
            size(size) {
        }
        datatype(std::string name, std::vector<pole> vec) :
            name(std::move(name)),
            size(0),
            poles(std::move(vec)) {
            for (const auto& it : poles)
                size += it.type->size;
            for (size_t i = 0; i != poles.size() - 1; i++)
                poles[i + i].offset = poles[i].offset + (ptrdiff_t) poles[i].type->size;
        }

        void add_pole(const std::string& pole_name, const datatype_ptr& type) {
            poles.emplace_back(pole_name, type, poles.empty() ? 0 : poles.back().offset + poles.back().type->size);
            size += type->size;
        }
    };

    template<typename T>
        requires std::constructible_from<datatype, std::string, T>
    datatype_ptr make_datatype(std::string name, T arg) {
        return std::make_shared<datatype>(std::move(name), std::move(arg));
    }
}
