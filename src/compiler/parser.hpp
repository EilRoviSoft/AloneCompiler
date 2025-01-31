#pragma once

//std
#include <functional>
#include <queue>
#include <string>
#include <variant>

//compiler
#include "compiler/context.hpp"
#include "compiler/datatypes.hpp"
#include "compiler/functions.hpp"
#include "compiler/tokens.hpp"

#define PARSER_ARGS size_t i, const token_vector& tokens, parse_queue& queue, funcs_queue& result

namespace amasm::compiler {
    class Parser {
        using parse_variant = std::variant<DatatypePtr, func_info>;
        using parse_queue = std::queue<std::tuple<std::string, parse_variant>>;

    public:
        explicit Parser(Context& ctx);

        funcs_queue parse(const token_vector& tokens) const;

    private:
        Context& _ctx;

        static auto make_parse_rule(const TokenType& type, std::function<size_t(const size_t&)> pred) {
            return std::make_pair(type, std::move(pred));
        }

        size_t _do_parse_logic(PARSER_ARGS) const;

        size_t _start_parse_struct(PARSER_ARGS) const;
        size_t _start_parse_func(PARSER_ARGS) const;
        size_t _parse_variable(PARSER_ARGS) const;
        size_t _parse_instruction(PARSER_ARGS) const;
        composed_inst _parse_generic_instruction(const func_info& func, PARSER_ARGS) const;
        size_t _finish_parse(PARSER_ARGS) const;
    };
}
