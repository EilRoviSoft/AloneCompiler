#include "layers.hpp"

//library
#include "library/general_functions.hpp"

//compiler_info
#include "compiler/info/scope_proxy.hpp"

namespace amasm::compiler {
    Layer::Layer(ScopeProxy& proxy):
        _proxy(&proxy) {
    }

    const IScopeElement* Layer::at(size_t hashed_key) const {
        return _data.at(hashed_key);
    }

    void Layer::add_ptr(ScopeElement&& elem) const {
        _proxy->add_ptr(std::move(elem));
    }

    void Layer::_emplace(size_t hashed_key, const IScopeElement* elem) {
        _data.emplace(hashed_key, elem);
    }

    const IScopeElement* layers_container::operator()(const layers_pos& pos, const std::string& key) const {
        size_t hashed_key = lib::hash_string(key);
        const auto& [w, h] = pos;
        return data[w][h].at(hashed_key);
    }
}
