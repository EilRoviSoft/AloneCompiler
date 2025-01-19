#include "parser.hpp"

//shared
#include "shared/types.hpp"

#define BIND_PARSE_CASE(TYPE, PRED) std::make_pair(TYPE, std::function<size_t(size_t, const token_vector&, parse_queue&, funcs_queue&)>(std::bind(PRED, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)))
#define RULE_STATEMENT(RULE_NAME, MESSAGE) if (!match_rule(_ctx, RULE_NAME, tokens, i)) { throw std::runtime_error(MESSAGE); }

namespace amasm::compiler {
    bool match_rule(const Context& ctx, const std::string& name, const token_vector& tokens, size_t begin) {
        const auto& pattern = ctx.get_rule(name);
        bool result = true;

        for (size_t i = 0; result && i < pattern.size(); i++)
            result = pattern[i] == tokens[i + begin].type;

        return result;
    }
    auto calc_offset(const datatype_ptr& type, const token_vector& tokens, size_t begin) {
        datatype_ptr curr_type = type;
        ptrdiff_t offset = 0;
        size_t i = 0;

        while (tokens[i + begin].type == TokenType::Dot) {
            const auto it = std::ranges::find_if(curr_type->poles, [&](const pole& pole) {
                return pole.name == tokens[i + begin + 2].literal;
            });
            curr_type = it->type;
            offset += it->offset;

            i += 2;
        }

        return std::make_tuple(offset, i);
    }

    // Parser

    Parser::Parser(Context& ctx) :
        _ctx(ctx) {
    }

    funcs_queue Parser::parse(const token_vector& tokens) const {
        funcs_queue result;
        parse_queue queue;

        for (size_t i = 0, di; i < tokens.size(); i += di, di = 0)
            di = _do_parse_logic(i, tokens, queue, result);

        return result;
    }

    size_t Parser::_do_parse_logic(PARSER_ARGS) const {
        using enum TokenType;

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
        func_info on_push;

        on_push.name = tokens[i + 2].literal;
        on_push.variables.inherit_from(_ctx.global_variables());
        // arguments dispatching up to rparen token
        for (j = i + 4; tokens[j].type != TokenType::RParen; j++) {
            if ((j - i) % 2) {
                if (tokens[j].type != TokenType::Comma)
                    throw std::runtime_error("wrong func definition");
            } else if (tokens[j].type == TokenType::Datatype) {
                on_push.args.emplace_back(_ctx.get_datatype(tokens[j].literal));
            } else
                throw std::runtime_error("wrong func definition");
        }
        on_push.args.shrink_to_fit();

        // return-type detecting ('cause it's optional)
        if (tokens[j + 1].type == TokenType::Colon) {
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
            const bool has_own_offset = tokens[i + 5].type == TokenType::Comma;

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

        auto& func = std::get<func_info>(data);
        size_t delta;
        inst_decl inst_decl;

        if (tokens[i].literal == "fcall") {
            size_t j = i + 1;

            inst_decl.name = "fcall";
            inst_decl.args.emplace_back(
                ArgumentType::JumpAddress,
                [&] {
                    std::string temp;
                    while (tokens[j].type != TokenType::Semicolon)
                        temp += tokens[j++].literal;
                    return temp;
                }(),
                0
            );
            delta = j - i + 1;
        } else
            std::tie(delta, inst_decl) = _parse_generic_instruction(func, i, tokens, queue, result);

        inst_decl.args.shrink_to_fit();
        func.lines.emplace_back(std::move(inst_decl));
        return delta;
    }

    composed_inst Parser::_parse_generic_instruction(const func_info& func, PARSER_ARGS) const {
        size_t j = i, args_n = 0;
        inst_decl inst_decl;
        const auto& inst_info = _ctx.get_inst(tokens[i].literal);
        bool flag = tokens[j + 1].type != TokenType::Semicolon;

        inst_decl.name = tokens[j].literal;
        j++;

        while (args_n < inst_info.max_args && flag) {
            argument_info on_emplace;

            if (match_rule(_ctx, "direct_argument", tokens, j)) {
                const auto& var = func.variables.get_variable(tokens[j].literal);
                auto [var_offset, delta] = calc_offset(var->type, tokens, j + 1);
                on_emplace = {
                    .type = var_offset ? ArgumentType::IndirectWithDisplacement : ArgumentType::Direct,
                    .name = tokens[j].literal,
                    .value = var_offset
                };
                j += delta + 1;
            } else if (tokens[j].type == TokenType::Number) {
                on_emplace = {
                    .type = ArgumentType::Immediate,
                    .name = "",
                    .value = std::stoll(tokens[j].literal)
                };
                j++;
            } else if (match_rule(_ctx, "indirect_argument", tokens, j)) {
                const auto& var = func.variables.get_variable(tokens[j + 1].literal);
                auto [var_offset, delta] = calc_offset(var->type, tokens, j + 2);
                on_emplace = {
                    .type = ArgumentType::IndirectWithDisplacement,
                    .name = tokens[j + 1].literal,
                    .value = var_offset
                };

                switch (tokens[j + delta + 2].type) {
                case TokenType::Plus:
                    on_emplace.value += std::stoll(tokens[j + delta + 3].literal);
                    delta += 1;
                    break;
                case TokenType::Minus:
                    on_emplace.value -= std::stoll(tokens[j + delta + 3].literal);
                    delta += 1;
                    break;
                default:
                    break;
                }

                j += delta + 3;
            } else if (args_n >= inst_info.min_args) {
                j += 1;
                break;
            } else
                throw std::runtime_error("wrong instruction definition");

            inst_decl.args.emplace_back(std::move(on_emplace));
            args_n++;

            // to add colon to diff
            if (tokens[j].type != TokenType::Semicolon)
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
            const auto func = std::get<func_info>(variant);
            result.emplace(func);
        }

        queue.pop();

        return 1;
    }
}
