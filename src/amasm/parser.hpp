#pragma once

//std
#include <queue>
#include <string>
#include <variant>
#include <vector>

//shared
#include "shared/bytecode.hpp"

//amasm
#include "amasm/info/arguments.hpp"
#include "amasm/info/datatypes.hpp"
#include "amasm/info/tokens.hpp"
#include "amasm/info/variables.hpp"

namespace alone::amasm {
    class AmasmContext;

    class Parser {
        struct parse_unit_t {
            std::string name;
            bool status;
        };
        struct edit_hint_t {
            size_t offset;
            std::string label_name;
        };

        struct inst_t {
            std::string name;
            std::vector<argument_t> args;
        };
        struct func_local_info_t {
            std::string name;
            datatype_ptr return_type;
            std::vector<datatype_ptr> args;
            Variables scope;
        };
        struct func_incomplete_signature_t {
            std::string fullname;
            shared::Bytecode bytecode;
            std::list<edit_hint_t> hints;
        };

        using parse_variant_t = std::variant<datatype_ptr, func_local_info_t>;
        using parse_queue_t = std::queue<std::tuple<std::string, bool, parse_variant_t>>;

    public:
        explicit Parser(const AmasmContext& amasm_ctx);

        shared::Bytecode parse(const token_array_t& tokens);

    private:
        const AmasmContext& _amasm_context;

        token_array_t _tokens;
        std::unordered_map<std::string, size_t> _labels;
        parse_queue_t _queue;
        size_t _index = 0, _delta = 0;
    };
}
