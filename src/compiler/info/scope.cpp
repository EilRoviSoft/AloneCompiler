#include "scope.hpp"

//library
#include "library/general_functions.hpp"

namespace amasm::compiler {
    Scope::~Scope() {
        _search.clear();
        _layers.clear();
        _container.clear();
    }

    const Datatype& Scope::get_datatype(const std::string& key) const {
        size_t hashed_key = lib::hash_string(key);
        return *dynamic_cast<const Datatype*>(_search.at(hashed_key));
    }
}
