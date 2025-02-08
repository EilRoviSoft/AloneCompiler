#include "compiler/context.hpp"

//std
#include <array>

//library
#include "library/general_functions.hpp"

//compiler_info
#include "compiler/info/datatype_builder.hpp"
#include "compiler/info/function_builder.hpp"
#include "compiler/info/inst_info_factory.hpp"
#include "compiler/info/variable_builder.hpp"

namespace amasm::compiler {
    // constructors

    Context::Context() :
        _proxy(_container) {
        _init();
        _reset();
    }

    // getters

    ScopeProxy Context::get_proxy() {
        return _proxy;
    }
    const std::unordered_map<std::string, TokenType>& Context::get_defined_tokens() const {
        return _defined_tokens;
    }
    const InstInfo& Context::get_inst_info(const std::string& key) const {
        auto hashed_key = lib::hash_string(key);
        return _instructions.at(hashed_key);
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
    void Context::_init_defined_tokens() {
        _defined_tokens = {
            { "this", TokenType::KwThis },
            { "var", TokenType::KwVar },
            { "section", TokenType::KwSection },
            { "label", TokenType::KwLabel },
            { "func", TokenType::KwFunc },
            { "struct", TokenType::KwStruct },
        };

        for (const auto& inst : _instructions | std::views::values)
            _defined_tokens.emplace(inst.name(), TokenType::InstName);
    }
    void Context::_init() {
        _init_instructions();
        _init_defined_tokens();
    }

    void Context::_init_datatypes() {
        _proxy.add(DatatypeBuilder().set_name("void").set_size(0).get_product());
        _proxy.add(DatatypeBuilder().set_name("uint8").set_size(1).get_product());
        _proxy.add(DatatypeBuilder().set_name("uint16").set_size(2).get_product());
        _proxy.add(DatatypeBuilder().set_name("uint32").set_size(4).get_product());
        _proxy.add(DatatypeBuilder().set_name("uint64").set_size(8).get_product());
        _proxy.add(DatatypeBuilder().set_name("int8").set_size(1).get_product());
        _proxy.add(DatatypeBuilder().set_name("int16").set_size(2).get_product());
        _proxy.add(DatatypeBuilder().set_name("int32").set_size(4).get_product());
        _proxy.add(DatatypeBuilder().set_name("int64").set_size(8).get_product());
    }
    void Context::_init_global_variables() {
        const std::array vars = {
            std::make_pair<std::string, std::vector<std::string>>("uint8",
                { "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh" }),
            std::make_pair<std::string, std::vector<std::string>>("uint16",
                { "ax", "bx", "cx", "dx", "si", "di", "ip", "bp", "sp", "flags", "gp" }),
            std::make_pair<std::string, std::vector<std::string>>("uint32",
                { "eax", "ebx", "ecx", "edx", "esi", "edi", "eip", "ebp", "esp", "eflags", "egp" }),
            std::make_pair<std::string, std::vector<std::string>>("uint64",
                { "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rip", "rbp", "rsp", "rflags", "rgp" }),
        };
        for (const auto& [type_name, collection] : vars)
            for (const auto& var_name : collection) {
                const auto& type = _proxy.get_datatype(type_name);
                auto var = VariableBuilder()
                          .set_name(var_name)
                          .set_address(lib::str_to_register(var_name))
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
