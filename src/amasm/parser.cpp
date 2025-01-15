#include "parser.hpp"

//amasm
#include "amasm/info/arguments.hpp"

#define BIND_PARSE_CASE(TYPE, PRED) std::make_pair(TYPE, std::function<size_t(size_t, const token_array_t&, parse_queue_t&, composed_funcs_t&)>(std::bind(PRED, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)))
#define RULE_STATEMENT(RULE_NAME, MESSAGE) if (!match_rule(_ctx, RULE_NAME, tokens, i)) { throw std::runtime_error(MESSAGE); }

namespace alone::amasm::inline parser_inlined {
    bool match_rule(const Context& ctx, const std::string& name, const token_array_t& tokens, size_t begin) {
        const auto& pattern = ctx.get_rule(name);
        bool result = true;

        for (size_t i = 0; result && i < pattern.size(); i++)
            result = pattern[i] == tokens[i + begin].type;

        return result;
    }
    auto calc_offset(const datatype_ptr& type, const token_array_t& tokens, size_t begin) {
        datatype_ptr curr_type = type;
        ptrdiff_t offset = 0;
        size_t i = 0;

        while (tokens[i + begin].type == Tokens::Dot) {
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

    composed_funcs_t Parser::parse(const token_array_t& tokens) const {
        composed_funcs_t result;
        parse_queue_t queue;

        for (size_t i = 0, di; i < tokens.size(); i += di, di = 0)
            di = _do_parse_logic(i, tokens, queue, result);

        return result;
    }

    size_t Parser::_do_parse_logic(PARSER_ARGS) const {
        using enum Tokens;

        static const std::unordered_map logic = {
            BIND_PARSE_CASE(KwStruct, &Parser::_start_parse_struct),
            BIND_PARSE_CASE(KwFunc, &Parser::_start_parse_func),
            BIND_PARSE_CASE(KwVar, &Parser::_parse_variable),
            BIND_PARSE_CASE(InstName, &Parser::_parse_instruction),
            BIND_PARSE_CASE(RBrace, &Parser::_finish_parse)
        };

        size_t delta;
        if (const auto it = logic.find(tokens[i].type); it != logic.end())
            delta = it->second(i, tokens, queue, result);
        else
            throw std::runtime_error("this parse rule doesn't exist");

        return delta;
    }

    size_t Parser::_start_parse_struct(PARSER_ARGS) const {
        RULE_STATEMENT("struct_define", "wrong struct definition")

        datatype_ptr ntype = make_datatype(tokens[i + 1].literal, 0);
        queue.emplace("struct_define", std::move(ntype));

        return 3;
    }
    size_t Parser::_start_parse_func(PARSER_ARGS) const {
        RULE_STATEMENT("func_define", "wrong func definition")

        size_t j, delta;
        func_info_t on_push;

        on_push.name = tokens[i + 2].literal;
        on_push.variables.inherit_from(_ctx.global_variables());
        // arguments dispatching up to rparen token
        for (j = i + 4; tokens[j].type != Tokens::RParen; j++) {
            if ((j - i) % 2) {
                if (tokens[j].type != Tokens::Comma)
                    throw std::runtime_error("wrong func definition");
            } else if (tokens[j].type == Tokens::Datatype) {
                on_push.args.emplace_back(_ctx.get_datatype(tokens[j].literal));
            } else
                throw std::runtime_error("wrong func definition");
        }
        on_push.args.shrink_to_fit();

        // return-type detecting ('cause it's optional)
        if (tokens[j + 1].type == Tokens::Colon) {
            on_push.return_type = _ctx.get_datatype(tokens[j + 2].literal);
            delta = j - i + 4;
        } else {
            on_push.return_type = _ctx.get_datatype("void");
            delta = j - i + 2;
        }
        queue.emplace("func_define", std::move(on_push));

        return delta;
    }

    size_t Parser::_parse_variable(PARSER_ARGS) const {
        RULE_STATEMENT("pole_define", "wrong pole definition")

        const auto& [name, data] = queue.front();
        size_t delta;

        if (name == "struct_define") {
            const auto& ntype = std::get<datatype_ptr>(data);
            const bool has_own_offset = tokens[i + 5].type == Tokens::Comma;

            ntype->add_pole(tokens[i + 2].literal, _ctx.get_datatype(tokens[i + 4].literal));
            delta = has_own_offset ? 13 : 6;
        } else
            throw std::runtime_error("isn't done yet");

        return delta;
    }

    size_t Parser::_parse_instruction(PARSER_ARGS) const {
        auto& [name, data] = queue.front();
        if (name != "func_define")
            throw std::runtime_error("wrong instruction definition placement");

        auto& func_info = std::get<func_info_t>(data);
        size_t delta;
        inst_decl_t inst_decl;

        if (tokens[i].literal == "fcall") {
            size_t j = i + 1;

            inst_decl.name = "fcall";
            inst_decl.args.resize(1);
            while (tokens[j].type != Tokens::Semicolon)
                inst_decl.args.front().name += tokens[j++].literal;
            delta = j - i + 1;
        } else
            std::tie(delta, inst_decl) = _parse_generic_instruction(func_info, i, tokens, queue, result);

        inst_decl.args.shrink_to_fit();
        func_info.lines.emplace_back(std::move(inst_decl));
        return delta;
    }

    composed_inst_t Parser::_parse_generic_instruction(const func_info_t& func_info, PARSER_ARGS) const {
        size_t j = i, args_n = 0;
        inst_decl_t inst_decl;
        const auto& inst_info = _ctx.get_inst(tokens[i].literal);
        bool flag = tokens[j + 1].type != Tokens::Semicolon;

        inst_decl.name = tokens[j].literal;
        j++;

        while (args_n < inst_info.max_args && flag) {
            argument_t on_emplace;

            if (match_rule(_ctx, "direct_argument", tokens, j)) {
                const auto& var = func_info.variables.get_variable(tokens[j + 1].literal);
                auto [var_offset, delta] = calc_offset(var->type, tokens, j + 2);
                on_emplace = {
                    .type = var_offset ? Arguments::IndirectWithDisplacement : Arguments::Direct,
                    .name = tokens[j + 1].literal,
                    .value = var_offset
                };
                j += delta + 2;
            } else if (tokens[j].type == Tokens::Number) {
                on_emplace = {
                    .type = Arguments::Immediate,
                    .name = "",
                    .value = std::stoll(tokens[j].literal)
                };
                j++;
            } else if (match_rule(_ctx, "indirect_argument", tokens, j)) {
                const auto& var = func_info.variables.get_variable(tokens[j + 2].literal);
                auto [var_offset, delta] = calc_offset(var->type, tokens, j + 3);
                on_emplace = {
                    .type = Arguments::IndirectWithDisplacement,
                    .name = tokens[j + 2].literal,
                    .value = var_offset
                };

                switch (tokens[j + delta + 3].type) {
                case Tokens::Plus:
                    on_emplace.value += std::stoll(tokens[j + delta + 4].literal);
                    delta += 2;
                    break;
                case Tokens::Minus:
                    on_emplace.value -= std::stoll(tokens[j + delta + 4].literal);
                    delta += 2;
                    break;
                default:
                    break;
                }

                j += delta + 4;
            } else if (args_n >= inst_info.min_args) {
                j += 1;
                break;
            } else
                throw std::runtime_error("wrong instruction definition");

            inst_decl.args.emplace_back(std::move(on_emplace));
            args_n++;

            // to add colon to diff
            if (tokens[j].type != Tokens::Semicolon)
                j++;
            else
                flag = false;
        }

        return { j - i + 1, inst_decl };
    }

    size_t Parser::_finish_parse(PARSER_ARGS) const {
        auto& [name, variant] = queue.front();

        if (name == "struct_define") {
            const auto type = std::get<datatype_ptr>(variant);
            _ctx.insert_datatype(type);
        } else if (name == "func_define") {
            const auto func = std::get<func_info_t>(variant);
            result.emplace(func);
        }

        queue.pop();

        return 1;
    }
}
