#pragma once

//std
#include <queue>
#include <string>
#include <variant>

//shared
#include "shared/bytecode.hpp"

//amasm
#include "amasm/info/context.hpp"
#include "amasm/info/datatypes.hpp"
#include "amasm/info/functions.hpp"
#include "amasm/info/tokens.hpp"

#define PARSER_ARGS size_t i, const token_array_t& tokens, parse_queue_t& queue, composed_funcs_t& result

namespace alone::amasm {
    class Parser {
        using parse_variant_t = std::variant<datatype_ptr, func_info_t>;
        using parse_queue_t = std::queue<std::tuple<std::string, parse_variant_t>>;

    public:
        explicit Parser(Context& ctx);

        composed_funcs_t parse(const token_array_t& tokens) const;

    private:
        Context& _ctx;

        static auto make_parse_rule(const Tokens& type, std::function<size_t(const size_t&)> pred) {
            return std::make_pair(type, std::move(pred));
        }

        size_t _do_parse_logic(PARSER_ARGS) const;

        size_t _start_parse_struct(PARSER_ARGS) const;
        size_t _start_parse_func(PARSER_ARGS) const;
        size_t _parse_variable(PARSER_ARGS) const;
        size_t _parse_instruction(PARSER_ARGS) const;
        composed_inst_t _parse_generic_instruction(const func_info_t& func_info, PARSER_ARGS) const;
        size_t _finish_parse(PARSER_ARGS) const;
    };
}
