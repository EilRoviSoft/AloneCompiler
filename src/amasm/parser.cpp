#include "parser.hpp"

#define BIND_PARSE_CASE(TYPE, PRED) std::make_pair(TYPE, std::function<size_t(PARSER_ARGS_BODY)>(std::bind(PRED, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)))
#define RULE_STATEMENT(RULE_NAME, MESSAGE) if (!match_rule(_ctx, RULE_NAME, tokens, i)) { throw std::runtime_error(MESSAGE); }

namespace alone::amasm::inline parser_inlined {
    bool match_rule(const Context& ctx, const std::string& name, const token_array_t& tokens, const size_t begin) {
        const auto& pattern = ctx.get_rule(name);
        bool result = true;

        for (size_t i = 0; result && i < pattern.size(); i++)
            result = pattern[i] == tokens[i + begin].type;

        return result;
    }
    auto calc_offset(const datatype_ptr& type, const token_array_t& tokens, const size_t begin) {
        datatype_ptr curr_type = type;
        ptrdiff_t offset = 0;
        size_t i = 0;

        while (tokens[i + begin].type == token_type::comma) {
            const auto it = std::ranges::find_if(curr_type->poles, [&](const pole_t& pole) {
                return pole.name == tokens[i + begin + 2].literal;
            });
            curr_type = it->type;
            offset += it->offset;

            i += 2;
        }

        return std::make_tuple(offset, i);
    }
}

namespace alone::amasm {
    Parser::Parser(Context& ctx) :
        _ctx(ctx) {
    }

    shared::Bytecode Parser::parse(const token_array_t& tokens) const {
        shared::Bytecode result;
        size_t summed_size = 8;
        parse_queue_t queue;
        labels_t labels;

        for (size_t i = 0, di, address = 0; i < tokens.size(); i += di, di = 0)
            di = _do_parse_logic(i, tokens, queue, labels);

        return result;
    }

    size_t Parser::_do_parse_logic(PARSER_ARGS_DEFINE) const {
        using enum token_type;

        static const std::unordered_map logic = {
            BIND_PARSE_CASE(kw_struct, &Parser::_start_parse_struct),
            BIND_PARSE_CASE(kw_func, &Parser::_start_parse_func),
            BIND_PARSE_CASE(kw_var, &Parser::_parse_variable),
            BIND_PARSE_CASE(inst_name, &Parser::_parse_instruction),
            BIND_PARSE_CASE(rbrace, &Parser::_finish_parse)
        };

        size_t delta;
        if (const auto it = logic.find(tokens[i].type); it != logic.end())
            delta = it->second(i, tokens, queue, labels);
        else
            throw std::runtime_error("this parse rule doesn't exist");

        return delta;
    }

    size_t Parser::_start_parse_struct(PARSER_ARGS_DEFINE) const {
        RULE_STATEMENT("struct_define", "wrong struct definition")

        datatype_ptr ntype = make_datatype(tokens[i + 1].literal, 0);
        queue.emplace("struct_define", false, std::move(ntype));

        return 3;
    }
    size_t Parser::_start_parse_func(PARSER_ARGS_DEFINE) const {
        RULE_STATEMENT("func_define", "wrong func definition")

        size_t j, delta;
        func_info_t on_push;

        on_push.name = tokens[i + 2].literal;
        // arguments dispatching up to rparen token
        for (j = i + 4; tokens[j].type != token_type::rparen; j++) {
            if ((j - i) % 2) {
                if (tokens[j].type != token_type::comma)
                    throw std::runtime_error("wrong func definition");
            } else if (tokens[j].type == token_type::datatype) {
                on_push.args.emplace_back(_ctx.get_datatype(tokens[j].literal));
            } else
                throw std::runtime_error("wrong func definition");
        }
        on_push.args.shrink_to_fit();

        // return-type detecting ('cause it's optional)
        if (tokens[j + 1].type == token_type::colon) {
            on_push.return_type = _ctx.get_datatype(tokens[j + 2].literal);
            delta = j - i + 4;
        } else {
            on_push.return_type = _ctx.get_datatype("void");
            delta = j - i + 2;
        }
        queue.emplace("func_define", false, std::move(on_push));

        return delta;
    }

    size_t Parser::_parse_variable(PARSER_ARGS_DEFINE) const {
        RULE_STATEMENT("pole_define", "wrong pole definition", 4)

        size_t delta;

        const auto& [name, is_finished, data] = queue.front();
        if (name == "struct_define") {
            const auto& ntype = std::get<datatype_ptr>(data);
            const bool has_own_offset = tokens[i + 5].type == token_type::comma;

            ntype->add_pole(tokens[i + 2].literal, _ctx.get_datatype(tokens[i + 4].literal));
            delta = has_own_offset ? 13 : 6;
        } else
            throw std::runtime_error("isn't done yet");

        return delta;
    }

    size_t Parser::_parse_instruction(PARSER_ARGS_DEFINE) const {
        auto& [name, is_finished, data] = queue.front();
        if (name != "func_define" || is_finished)
            throw std::runtime_error("wrong instruction definition placement");

        auto& func_data = std::get<func_info_t>(data);
        size_t j = i + 1;
        inst_decl_t inst_decl;
        const auto& inst_info = _ctx.get_inst(tokens[i].literal);

        inst_decl.name = tokens[i].literal;
        if (inst_decl.name == "fcall") {
            inst_decl.args.resize(1);
            while (tokens[j].type != token_type::semicolon) {
                inst_decl.args.front().name += tokens[j].literal;
                j++;
            }
        } else {
            size_t dj, args_n = 0;
            while (args_n < inst_info.max_args && tokens[j].type != token_type::semicolon) {
                argument_t on_emplace;

                if (tokens[j].type != token_type::inst_name && tokens[j].type != token_type::comma)
                    throw std::runtime_error("wrong instruction's arguments definition");

                if (match_rule(_ctx, "direct_argument", tokens, j + 1)) {
                    auto [var_offset, delta] = calc_offset(_ctx.get_datatype(inst_decl.name), tokens, j + 3);
                    on_emplace = {
                        .type = var_offset ? argument_type::indirect_with_displacement : argument_type::direct,
                        .name = tokens[j + 2].literal,
                        .value = var_offset
                    };
                    dj = delta + 3;
                } else if (tokens[j + 1].type == token_type::number) {
                    on_emplace = {
                        .type = argument_type::immediate,
                        .name = "",
                        .value = std::stoll(tokens[j + 1].literal)
                    };
                    dj = 2;
                } else if (match_rule(_ctx, "indirect_argument", tokens, j + 1)) {
                    auto [var_offset, delta] = calc_offset(_ctx.get_datatype(inst_decl.name), tokens, j + 4);
                    on_emplace = {
                        .type = argument_type::indirect_with_displacement,
                        .name = tokens[j + 3].literal,
                        .value = var_offset
                    };

                    switch (tokens[j + delta + 4].type) {
                    case token_type::plus:
                        on_emplace.value += std::stoll(tokens[j + delta + 5].literal);
                        delta += 2;
                        break;
                    case token_type::minus:
                        on_emplace.value -= std::stoll(tokens[j + delta + 5].literal);
                        delta += 2;
                        break;
                    default:
                        break;
                    }

                    dj = delta + 5;
                } else if (args_n >= inst_info.min_args) {
                    j += 1;
                    break;
                } else
                    throw std::runtime_error("wrong instruction definition");

                inst_decl.args.emplace_back(std::move(on_emplace));
                j += dj;
                args_n++;
            }
        }

        inst_decl.args.shrink_to_fit();
        func_data.lines.emplace_back(std::move(inst_decl));
        return j - i + 1;
    }
    size_t Parser::_finish_parse(PARSER_ARGS_DEFINE) const {
        return 0;
    }
}
