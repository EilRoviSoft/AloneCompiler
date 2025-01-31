#pragma once

//std
#include <concepts>
#include <memory>
#include <string>
#include <vector>

namespace amasm::compiler {
    class Datatype {
    public:
        struct pole {
            std::string name;
            ptrdiff_t offset;
            const Datatype& type;

            pole(std::string name, ptrdiff_t offset, const Datatype& type);
        };

        Datatype(std::string name, size_t size);
        Datatype(std::string name, std::vector<pole> vec);

        const std::string& name() const;
        size_t size() const;
        const std::vector<pole>& poles() const;

        void add_pole(const std::string& pole_name, const Datatype* type);

    private:
        std::string _name;
        size_t _size;
        std::vector<pole> _poles;
    };
    using DatatypePtr = std::shared_ptr<Datatype>;

    template<typename T>
        requires std::constructible_from<Datatype, std::string, T>
    DatatypePtr make_datatype(std::string name, T arg) {
        return std::make_shared<Datatype>(std::move(name), std::move(arg));
    }
}
