#include "context.hpp"

//std
#include <ranges>

//shared
#include "shared/general_functions.hpp"
#include "shared/types.hpp"

#define MAKE(NAME, TYPE, ADDRESS) make_variable(NAME, get_datatype(TYPE), ADDRESS)

namespace amasm::compiler {
    // constructors

    Context::Context() {
        _init();
    }

    // editors

    void Context::insert_datatype(const datatype_ptr& ptr) {
        _datatypes.emplace(shared::hash_string(ptr->name), ptr);
    }

    // getters

    const TokenType& Context::get_token_or(const std::string& key, const TokenType& default_token) const {
        const auto it = _defined_tokens.find(key);
        return it != _defined_tokens.end() ? it->second : default_token;
    }
    datatype_ptr Context::get_datatype(const std::string& key) const {
        const auto it = _datatypes.find(shared::hash_string(key));
        return it != _datatypes.end() ? it->second : nullptr;
    }
    const std::vector<TokenType>& Context::get_rule(const std::string& key) const {
        return _rules.at(key);
    }
    const inst_info& Context::get_inst(const std::string& key) {
        return _instructions.at(shared::hash_string(key));
    }
    const VariablesCollection& Context::global_variables() const {
        return _global_variables;
    }

    // initializers

    void Context::_init() {
        _init_defined_tokens();
        _init_datatypes();
        _init_rules();
        _init_surrounded_by_braces_signatures();
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
        static auto wrap = [](const datatype_ptr& ptr) {
            return std::make_pair(shared::hash_string(ptr->name), ptr);
        };
        _datatypes = {
            wrap(make_datatype("void", 0)),
            wrap(make_datatype("uint8", 1)),
            wrap(make_datatype("uint16", 2)),
            wrap(make_datatype("uint32", 4)),
            wrap(make_datatype("uint64", 8)),
            wrap(make_datatype("int8", 1)),
            wrap(make_datatype("int16", 2)),
            wrap(make_datatype("int32", 4)),
            wrap(make_datatype("int64", 8)),
            wrap(make_datatype("float32", 4)),
            wrap(make_datatype("float64", 8)),
        };

        for (const auto& value : _datatypes | std::views::values)
            _defined_tokens.emplace(value->name, TokenType::Datatype);
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
    void Context::_init_surrounded_by_braces_signatures() {
        _surrounded_by_braces_signatures = {
            "struct_define", "func_define"
        };
    }
    void Context::_init_instructions() {
        auto inst_collection = generate_isa_info();

        for (auto&& it : inst_collection) {
            const auto key = shared::hash_string(it.name);
            _instructions.emplace(key, std::move(it));
        }

        for (const auto& value : _instructions | std::views::values)
            _defined_tokens.emplace(value.name, TokenType::InstName);
    }
    void Context::_init_global_variables() {
        _global_variables.inherit_from({
            MAKE("al", "uint8", AL),
            MAKE("ah", "uint8", AH),
            MAKE("bl", "uint8", BL),
            MAKE("bh", "uint8", BH),
            MAKE("cl", "uint8", CL),
            MAKE("ch", "uint8", CH),
            MAKE("dl", "uint8", DL),
            MAKE("dh", "uint8", DH),

            MAKE("ax", "uint16", AX),
            MAKE("bx", "uint16", BX),
            MAKE("cx", "uint16", CX),
            MAKE("dx", "uint16", DX),
            MAKE("ipx", "uint16", IPX),
            MAKE("bpx", "uint16", BPX),
            MAKE("spx", "uint16", SPX),
            MAKE("flags", "uint16", FLAGS),
            MAKE("gpx", "uint16", GPX),

            MAKE("eax", "uint32", EAX),
            MAKE("ebx", "uint32", EBX),
            MAKE("ecx", "uint32", ECX),
            MAKE("edx", "uint32", EDX),
            MAKE("eip", "uint32", EIP),
            MAKE("ebp", "uint32", EBP),
            MAKE("esp", "uint32", ESP),
            MAKE("eflags", "uint32", EFLAGS),
            MAKE("egp", "uint32", EGP),

            MAKE("rax", "uint64", RAX),
            MAKE("rbx", "uint64", RBX),
            MAKE("rcx", "uint64", RCX),
            MAKE("rdx", "uint64", RDX),
            MAKE("rip", "uint64", RIP),
            MAKE("rbp", "uint64", RBP),
            MAKE("rsp", "uint64", RSP),
            MAKE("rflags", "uint64", RFLAGS),
            MAKE("rgp", "uint64", RGP),
        });
    }
}
