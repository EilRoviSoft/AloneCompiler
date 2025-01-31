#include "datatypes.hpp"

namespace amasm::compiler {
    // pole

    Datatype::pole::pole(std::string name, ptrdiff_t offset, const Datatype& type) :
        name(std::move(name)),
        offset(offset),
        type(type) {
    }

    // Datatype

    Datatype::Datatype(std::string name, size_t size):
        _name(std::move(name)),
        _size(size) {
    }
    Datatype::Datatype(std::string name, std::vector<pole> vec):
        _name(std::move(name)),
        _size(0),
        _poles(std::move(vec)) {
        for (const auto& it : _poles)
            _size += it.type._size;
        for (size_t i = 0; i != _poles.size() - 1; i++)
            _poles[i + i].offset = _poles[i].offset + (ptrdiff_t) _poles[i].type._size;
    }
    const std::string& Datatype::name() const {
        return _name;
    }
    size_t Datatype::size() const {
        return _size;
    }
    const std::vector<Datatype::pole>& Datatype::poles() const {
        return _poles;
    }

    void Datatype::add_pole(const std::string& pole_name, const Datatype* type) {
        const ptrdiff_t offset = _poles.empty() ? 0 : _poles.back().offset + _poles.back().type._size;
        _poles.emplace_back(pole_name, offset, *type);
        _size += type->_size;
    }
}
