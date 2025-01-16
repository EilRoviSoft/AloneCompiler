#include "context.hpp"

//std
#include <ranges>

//shared
#include "shared/general_functions.hpp"
#include "shared/registers.hpp"

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

    const Tokens& Context::get_token_or(const std::string& key, const Tokens& default_token) const {
        const auto it = _defined_tokens.find(key);
        return it != _defined_tokens.end() ? it->second : default_token;
    }
    datatype_ptr Context::get_datatype(const std::string& key) const {
        const auto it = _datatypes.find(shared::hash_string(key));
        return it != _datatypes.end() ? it->second : nullptr;
    }
    const std::vector<Tokens>& Context::get_rule(const std::string& key) const {
        return _rules.at(key);
    }
    const inst_info_t& Context::get_inst(const std::string& key) {
        return _instructions.at(shared::hash_string(key));
    }
    const Variables& Context::global_variables() const {
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
            { "this", Tokens::KwThis },
            { "var", Tokens::KwVar },
            { "section", Tokens::KwSection },
            { "label", Tokens::KwLabel },
            { "func", Tokens::KwFunc },
            { "struct", Tokens::KwStruct },
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
            _defined_tokens.emplace(value->name, Tokens::Datatype);
    }
    void Context::_init_rules() {
        using enum Tokens;
        _rules = {
            { "struct_define", { KwStruct, Identifier, LBrace } },
            { "pole_define", { KwVar, Percent, Identifier, Comma, Datatype } },
            { "func_define", { KwFunc, At, Identifier, LParen } },
            { "direct_argument", { Percent, Identifier } },
            { "indirect_argument", { LBracket, Percent, Identifier } }
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
            _defined_tokens.emplace(value.name, Tokens::InstName);
    }
    void Context::_init_global_variables() {
        using enum shared::Registers;
        _global_variables.inherit_from({
            make_variable("al", get_datatype("uint8"), (shared::address_t) AL),
            make_variable("ah", get_datatype("uint8"), (shared::address_t) AH),
            make_variable("bl", get_datatype("uint8"), (shared::address_t) BL),
            make_variable("bh", get_datatype("uint8"), (shared::address_t) BH),
            make_variable("cl", get_datatype("uint8"), (shared::address_t) CL),
            make_variable("ch", get_datatype("uint8"), (shared::address_t) CH),
            make_variable("dl", get_datatype("uint8"), (shared::address_t) DL),
            make_variable("dh", get_datatype("uint8"), (shared::address_t) DH),

            make_variable("ax", get_datatype("uint16"), (shared::address_t) AX),
            make_variable("bx", get_datatype("uint16"), (shared::address_t) BX),
            make_variable("cx", get_datatype("uint16"), (shared::address_t) CX),
            make_variable("dx", get_datatype("uint16"), (shared::address_t) DX),
            make_variable("ip", get_datatype("uint16"), (shared::address_t) IP),
            make_variable("bp", get_datatype("uint16"), (shared::address_t) BP),
            make_variable("cp", get_datatype("uint16"), (shared::address_t) CP),
            make_variable("sp", get_datatype("uint16"), (shared::address_t) SP),
            make_variable("flags", get_datatype("uint16"), (shared::address_t) FLAGS),
            make_variable("gp", get_datatype("uint16"), (shared::address_t) GP),

            make_variable("eax", get_datatype("uint32"), (shared::address_t) EAX),
            make_variable("ebx", get_datatype("uint32"), (shared::address_t) EBX),
            make_variable("ecx", get_datatype("uint32"), (shared::address_t) ECX),
            make_variable("edx", get_datatype("uint32"), (shared::address_t) EDX),
            make_variable("eip", get_datatype("uint32"), (shared::address_t) EIP),
            make_variable("ebp", get_datatype("uint32"), (shared::address_t) EBP),
            make_variable("ecp", get_datatype("uint32"), (shared::address_t) ECP),
            make_variable("esp", get_datatype("uint32"), (shared::address_t) ESP),
            make_variable("eflags", get_datatype("uint32"), (shared::address_t) EFLAGS),
            make_variable("egp", get_datatype("uint32"), (shared::address_t) EGP),

            make_variable("rax", get_datatype("uint64"), (shared::address_t) RAX),
            make_variable("rbx", get_datatype("uint64"), (shared::address_t) RBX),
            make_variable("rcx", get_datatype("uint64"), (shared::address_t) RCX),
            make_variable("rdx", get_datatype("uint64"), (shared::address_t) RDX),
            make_variable("rip", get_datatype("uint64"), (shared::address_t) RIP),
            make_variable("rbp", get_datatype("uint64"), (shared::address_t) RBP),
            make_variable("rcp", get_datatype("uint64"), (shared::address_t) RCP),
            make_variable("rsp", get_datatype("uint64"), (shared::address_t) RSP),
            make_variable("rflags", get_datatype("uint64"), (shared::address_t) RFLAGS),
            make_variable("rgp", get_datatype("uint64"), (shared::address_t) RGP),
        });
    }
}
