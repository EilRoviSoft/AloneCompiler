#include "context.hpp"

//library
#include "library/general_functions.hpp"

//compiler_info
#include "compiler/info/datatype_builder.hpp"
#include "compiler/info/function_builder.hpp"
#include "compiler/info/instruction_factory.hpp"
#include "compiler/info/variable_builder.hpp"

namespace amasm::compiler {
    // constructors

    Context::Context() :
        _proxy(_scope) {
    }

    // getters

    ScopeProxy& Context::global_proxy() {
        return _proxy;
    }
    const std::unordered_map<std::string, TokenType>& Context::get_defined_tokens() const {
        return _defined_tokens;
    }
    const std::vector<TokenType>& Context::get_rule(const std::string& key) const {
        return _rules.at(key);
    }
    const InstInfo& Context::get_inst(const std::string& key) {
        return _instructions.at(lib::hash_string(key));
    }

    // initializers

    void Context::_init() {
        _init_defined_tokens();
        _init_datatypes();
        _init_rules();
        _init_surrounded_by_braces();
        _init_instructions();
        _init_global_variables();
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
    }
    void Context::_init_datatypes() {
        _proxy.add(DatatypeBuilder().name("void").size(0).build());
        _proxy.add(DatatypeBuilder().name("uint8").size(1).build());
        _proxy.add(DatatypeBuilder().name("uint16").size(2).build());
        _proxy.add(DatatypeBuilder().name("uint32").size(4).build());
        _proxy.add(DatatypeBuilder().name("uint64").size(8).build());
        _proxy.add(DatatypeBuilder().name("int8").size(1).build());
        _proxy.add(DatatypeBuilder().name("int16").size(2).build());
        _proxy.add(DatatypeBuilder().name("int32").size(4).build());
        _proxy.add(DatatypeBuilder().name("int64").size(8).build());

        for (const auto& v : _proxy.get_all_datatypes())
            _defined_tokens.emplace(v->name(), TokenType::Datatype);
    }
    void Context::_init_rules() {
        using enum TokenType;
        _rules = {
            { "struct_define", { KwStruct, Identifier, LBrace } },
            { "pole_define", { KwVar, Identifier, Comma, Datatype } },
            { "func_define", { KwFunc, At, Identifier, LParen } },
            { "direct_argument", { Identifier } },
            { "indirect_argument", { LBracket, Identifier } }
        };
    }
    void Context::_init_surrounded_by_braces() {
        _surrounded_by_braces = {
            "struct_define", "func_define"
        };
    }
    void Context::_init_instructions() {
        auto inst_collection = InstInfoFactory().generate_info().make();

        for (const auto& v : inst_collection)
            _defined_tokens.emplace(v.name(), TokenType::InstName);

        for (auto&& it : inst_collection) {
            size_t hashed_key = it.hash();
            _instructions.emplace(hashed_key, std::move(it));
        }
    }
    void Context::_init_global_variables() {
        auto make = [this](std::string&& name, lib::address address, const std::string& type_name) {
            const auto& type = _proxy.get_datatype(type_name);
            return VariableBuilder().name(std::move(name)).address(address).datatype(type).build();
        };

        _proxy.add(make("al", AL, "uint8"));
        _proxy.add(make("ah", AH, "uint8"));
        _proxy.add(make("bl", BL, "uint8"));
        _proxy.add(make("bh", BH, "uint8"));
        _proxy.add(make("cl", CL, "uint8"));
        _proxy.add(make("ch", CH, "uint8"));
        _proxy.add(make("dl", DL, "uint8"));
        _proxy.add(make("dh", DH, "uint8"));

        _proxy.add(make("ax", AX, "uint16"));
        _proxy.add(make("bx", BX, "uint16"));
        _proxy.add(make("cx", CX, "uint16"));
        _proxy.add(make("dx", DX, "uint16"));
        _proxy.add(make("ipx", IPX, "uint16"));
        _proxy.add(make("bpx", BPX, "uint16"));
        _proxy.add(make("spx", SPX, "uint16"));
        _proxy.add(make("flags", FLAGS, "uint16"));
        _proxy.add(make("gpx", GPX, "uint16"));

        _proxy.add(make("eax", EAX, "uint32"));
        _proxy.add(make("ebx", EBX, "uint32"));
        _proxy.add(make("ecx", ECX, "uint32"));
        _proxy.add(make("edx", EDX, "uint32"));
        _proxy.add(make("eip", EIP, "uint32"));
        _proxy.add(make("ebp", EBP, "uint32"));
        _proxy.add(make("esp", ESP, "uint32"));
        _proxy.add(make("eflags", EFLAGS, "uint32"));
        _proxy.add(make("egp", EGP, "uint32"));

        _proxy.add(make("rax", RAX, "uint64"));
        _proxy.add(make("rbx", RBX, "uint64"));
        _proxy.add(make("rcx", RCX, "uint64"));
        _proxy.add(make("rdx", RDX, "uint64"));
        _proxy.add(make("rip", RIP, "uint64"));
        _proxy.add(make("rbp", RBP, "uint64"));
        _proxy.add(make("rsp", RSP, "uint64"));
        _proxy.add(make("rflags", RFLAGS, "uint64"));
        _proxy.add(make("rgp", RGP, "uint64"));
    }
}
