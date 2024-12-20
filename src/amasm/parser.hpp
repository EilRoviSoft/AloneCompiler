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

    // TODO: make queue and tokens local variables
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
        struct func_info_t {
            std::string name;
            datatype_ptr return_type;
            std::vector<datatype_ptr> args;
            Variables scope;
        };
        /*struct func_incomplete_signature_t {
            std::string fullname;
            shared::Bytecode bytecode;
            std::list<edit_hint_t> hints;
        };*/

        using parse_variant_t = std::variant<datatype_ptr, func_info_t>;
        using parse_queue_t = std::queue<std::tuple<std::string, bool, parse_variant_t>>;

    public:
        explicit Parser(AmasmContext& ctx);

        // TODO: return queue of functions' signatures
        shared::Bytecode parse(token_array_t tokens);

    private:
        AmasmContext& _ctx;

        token_array_t _tokens;
        std::unordered_map<std::string, size_t> _labels;
        parse_queue_t _queue;

        static auto make_parse_rule(const token_type& type, std::function<size_t(const size_t&)> pred) {
            return std::make_pair(type, std::move(pred));
        }

        size_t _do_parse_logic(const size_t& i);

        size_t _start_parse_struct(const size_t& i);
        size_t _start_parse_func(const size_t& i);
        size_t _parse_variable(const size_t& i);
        size_t _parse_instruction(const size_t& i);
        size_t _parse_generic_instruction(const size_t& i);
        size_t _parse_fcall_instruction(const size_t& i);
        size_t _finish_parse(const size_t& i);
    };
}
