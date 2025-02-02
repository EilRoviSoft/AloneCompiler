#include "scope.hpp"

namespace amasm::compiler {
    Scope::~Scope() {
        _search.clear();
        _layers.data.clear();
        _container.clear();
    }
}
