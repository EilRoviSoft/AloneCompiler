#include "compiler/context.hpp"

//std
#include <array>

//frozen
#include "frozen/string.h"
#include "frozen/unordered_map.h"

//library
#include "library/general_functions.hpp"

//compiler_info
#include "compiler/info/datatype_builder.hpp"
#include "compiler/info/function_builder.hpp"
#include "compiler/info/inst_info_factory.hpp"
#include "compiler/info/variable_builder.hpp"

namespace amasm::compiler::parser {
    static constexpr frozen::unordered_map<frozen::string, size_t, 9> predefined_datatypes = {
        { "void", 0 },
        { "uint8", 1 },
        { "uint16", 2 },
        { "uint32", 4 },
        { "uint64", 8 },
        { "int8", 1 },
        { "int16", 2 },
        { "int32", 4 },
        { "int64", 8 }
    };
    static const std::array predefined_vars = {
        std::make_pair<std::string, std::vector<std::string>>("uint8",
            { "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh" }),
        std::make_pair<std::string, std::vector<std::string>>("uint16",
            { "ax", "bx", "cx", "dx", "si", "di", "ip", "bp", "sp", "flags", "gp" }),
        std::make_pair<std::string, std::vector<std::string>>("uint32",
            { "eax", "ebx", "ecx", "edx", "esi", "edi", "eip", "ebp", "esp", "eflags", "egp" }),
        std::make_pair<std::string, std::vector<std::string>>("uint64",
            { "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rip", "rbp", "rsp", "rflags", "rgp" })
    };
}

namespace amasm::compiler {
    // constructors

    Context::Context() :
        _proxy(_container) {
        _init_instructions();
        _reset();
    }

    // getters

    ScopeProxy Context::get_proxy() {
        return _proxy;
    }
    const InstInfo& Context::get_inst(const std::string& key) const {
        auto hashed_key = lib::hash_string(key);
        return _instructions.at(hashed_key);
    }

    // lookup

    bool Context::has_inst(const std::string& key) const {
        auto hashed_key = lib::hash_string(key);
        return _instructions.contains(hashed_key);
    }

    // copy data

    ScopeContainer Context::release_container() {
        auto result = _container.clone();
        _reset();
        return result;
    }

    // initializers

    void Context::_init_instructions() {
        auto inst_collection = InstInfoFactory().generate_info().get_product();

        while (!inst_collection.empty()) {
            auto item = std::move(inst_collection.front());
            inst_collection.pop_front();
            auto hashed_key = item.hash();
            _instructions.emplace(hashed_key, std::move(item));
        }
    }

    void Context::_init_datatypes() {
        for (const auto& [name, size] : parser::predefined_datatypes) {
            auto datatype = DatatypeBuilder()
                .set_name(name.data())
                .set_size(size)
                .get_product();
            _proxy.add(std::move(datatype));
        }
    }
    void Context::_init_global_variables() {
        for (const auto& [type_name, collection] : parser::predefined_vars)
            for (const auto& var_name : collection) {
                const auto& type = _proxy.get_datatype(type_name);
                auto var = VariableBuilder()
                    .set_name(var_name)
                    .set_fixed_address(lib::str_to_register(var_name))
                    .set_datatype(type)
                    .get_product();
                _proxy.add(std::move(var));
            }
    }
    void Context::_reset() {
        _container = ScopeContainer();
        _init_datatypes();
        _init_global_variables();
    }
}
