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

#define PARSER_ARGS_BODY size_t, const token_array_t&, parse_queue_t&, labels_t&
#define PARSER_ARGS_DEFINE size_t i, const token_array_t& tokens, parse_queue_t& queue, labels_t& labels
//#define PARSER_LOCAL_ARGS_TUPLE i, tokens, queue, labels

namespace alone::amasm {
    class Context;

    struct inst_decl_t {
        std::string name;
        std::vector<argument_t> args;
    };

    class Parser {
        struct parse_unit_t {
            std::string name;
            bool status;
        };
        struct edit_hint_t {
            size_t offset;
            std::string label_name;
        };

        struct func_info_t {
            std::string name;
            datatype_ptr return_type;
            std::vector<datatype_ptr> args;
            Variables variables;
            std::vector<inst_decl_t> lines;
        };
        /*struct func_incomplete_signature_t {
            std::string fullname;
            shared::Bytecode bytecode;
            std::list<edit_hint_t> hints;
        };*/

        using parse_variant_t = std::variant<datatype_ptr, func_info_t>;
        using parse_queue_t = std::queue<std::tuple<std::string, bool, parse_variant_t>>;
        using labels_t = std::unordered_map<std::string, size_t>;

    public:
        explicit Parser(Context& ctx);

        // TODO: return queue of functions' signatures
        shared::Bytecode parse(const token_array_t& tokens) const;

    private:
        Context& _ctx;

        static auto make_parse_rule(const token_type& type, std::function<size_t(const size_t&)> pred) {
            return std::make_pair(type, std::move(pred));
        }

        size_t _do_parse_logic(PARSER_ARGS_DEFINE) const;

        size_t _start_parse_struct(PARSER_ARGS_DEFINE) const;
        size_t _start_parse_func(PARSER_ARGS_DEFINE) const;
        size_t _parse_variable(PARSER_ARGS_DEFINE) const;
        // TODO: make separate methods for parsing fcall and other instructions
        size_t _parse_instruction(PARSER_ARGS_DEFINE) const;
        /*std::tuple<size_t, inst_decl_t> _parse_generic_instruction(PARSER_ARGS_DEFINE) const;
        std::tuple<size_t, inst_decl_t> _parse_fcall_instruction(PARSER_ARGS_DEFINE) const;*/
        size_t _finish_parse(PARSER_ARGS_DEFINE) const;
    };
}
