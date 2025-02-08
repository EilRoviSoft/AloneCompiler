#include "parser.hpp"

//std
#include <functional>
#include <stdexcept>

//compiler_info
#include "compiler/info/instruction_builder.hpp"
#include "compiler/info/variable_builder.hpp"

#define RULE_STATEMENT(RULE_NAME, MESSAGE) if (!match(ctx.get_rule(RULE_NAME), tokens, i)) throw std::runtime_error(MESSAGE);

namespace amasm::compiler {
    // service functions

    auto match(const std::vector<TokenType>& rule, const token_vector& tokens, size_t begin) {
        bool result = true;

        for (size_t i = 0; result && i < rule.size(); i++)
            result = rule[i] == tokens[i + begin].type;

        return result;
    }
    auto calc_offset(const Datatype& type, const token_vector& tokens, size_t begin) {
        const Datatype* curr_type = &type;
        ptrdiff_t offset = 0;
        size_t i = 0;

        while (tokens[i + begin].type == TokenType::Dot) {
            const auto& pole = curr_type->pole(tokens[i + begin + 2].literal);
            curr_type = pole.type;
            offset += pole.offset;
            i += 2;
        }

        return std::make_tuple(offset, i);
    }
    auto make_parse_rule(const TokenType& type, std::function<size_t(const size_t&)> pred) {
        return std::make_pair(type, std::move(pred));
    }

    // parsing functions

    size_t start_parse_datatype(Context& ctx, size_t i, const token_vector& tokens, parse_queue& queue) {
        RULE_STATEMENT("struct_define", "wrong struct definition")

        queue.emplace(
            "datatype_builder",
            std::move(DatatypeBuilder().set_name(tokens[i + 1].literal))
        );

        return 3;
    }
    size_t start_parse_function(Context& ctx, size_t i, const token_vector& tokens, parse_queue& queue) {
        RULE_STATEMENT("func_define", "wrong func definition")

        size_t j, delta;
        FunctionBuilder builder;
        auto scopes = ctx.get_proxy();

        builder.set_name(tokens[i + 2].literal)
               .set_scope(scopes, scopes.amount());

        // arguments dispatching up to rparen token
        for (j = i + 4; tokens[j].type != TokenType::RParen; j++) {
            if ((j - i) % 2) {
                if (tokens[j].type != TokenType::Comma)
                    throw std::runtime_error("wrong func definition");
            } else if (tokens[j].type == TokenType::Identifier) {
                builder.add_argument_type(scopes.get_datatype(tokens[j].literal));
            } else
                throw std::runtime_error("wrong func definition");
        }

        // return-type detecting ('cause it's optional)
        if (tokens[j + 1].type == TokenType::Colon) {
            builder.set_return_type(scopes.get_datatype(tokens[j + 2].literal));
            delta = j - i + 4;
        } else {
            builder.set_return_type(scopes.get_datatype("void"));
            delta = j - i + 2;
        }

        queue.emplace("function_builder", std::move(builder));
        return delta;
    }
    size_t parse_variable(Context& ctx, size_t i, const token_vector& tokens, parse_queue& queue) {
        RULE_STATEMENT("pole_define", "wrong pole definition")

        size_t delta;
        auto& [name, data] = queue.front();
        auto scopes = ctx.get_proxy();

        if (name == "datatype_builder") {
            auto& builder = std::get<DatatypeBuilder>(data);
            bool has_own_offset = tokens[i + 5].type == TokenType::Comma;

            builder.add_pole(tokens[i + 2].literal, scopes.get_datatype(tokens[i + 4].literal));
            delta = has_own_offset ? 13 : 6;
        } else
            throw std::runtime_error("isn't done yet");

        return delta;
    }
    size_t parse_instruction(Context& ctx, size_t i, const token_vector& tokens, parse_queue& queue) {
        auto& [name, data] = queue.front();
        if (name != "function_builder")
            throw std::runtime_error("wrong instruction definition placement");

        size_t j = i + 1, di;
        auto& func_builder = std::get<FunctionBuilder>(data);
        auto scopes = ctx.get_proxy();
        auto scope_id = scopes.amount() - 1;
        InstDeclBuilder inst_builder;

        inst_builder.set_info(ctx.get_inst_info(tokens[i].literal));
        if (tokens[i].literal == "fcall") {
            argument_info on_add = {
                .name = "",
                .value = 0,
                .type = ArgumentType::JumpAddress
            };
            while (tokens[j].type != TokenType::Semicolon) {
                on_add.name += tokens[j].literal;
                j++;
            }

            inst_builder.add_argument(std::move(on_add));
            di = j - i + 1;
        } else {
            size_t args_n = 0;
            bool flag = tokens[j].type != TokenType::Semicolon;
            const auto& inst_info = ctx.get_inst_info(tokens[i].literal);

            while (args_n < inst_info.max_args() && flag) {
                argument_info on_add;

                if (match(ctx.get_rule("direct_argument"), tokens, j)) {
                    const auto& var = scopes.get_variable(tokens[j].literal, scope_id);
                    auto [var_offset, dj] = calc_offset(var.datatype(), tokens, j + 1);
                    on_add = {
                        .name = tokens[j].literal,
                        .value = var_offset,
                        .type = var_offset ? ArgumentType::IndirectWithDisplacement : ArgumentType::Direct
                    };
                    j += dj + 1;
                } else if (tokens[j].type == TokenType::Number) {
                    on_add = {
                        .name = "",
                        .value = std::stoll(tokens[j].literal),
                        .type = ArgumentType::Immediate
                    };
                    j++;
                } else if (match(ctx.get_rule("indirect_argument"), tokens, j)) {
                    const auto& var = scopes.get_variable(tokens[j + 1].literal, scope_id);
                    auto [var_offset, dj] = calc_offset(var.datatype(), tokens, j + 2);
                    on_add = {
                        .name = tokens[j + 1].literal,
                        .value = var_offset,
                        .type = ArgumentType::IndirectWithDisplacement
                    };

                    switch (tokens[j + dj + 2].type) {
                    case TokenType::Plus:
                        on_add.value += std::stoll(tokens[j + dj + 3].literal);
                        dj += 1;
                        break;
                    case TokenType::Minus:
                        on_add.value -= std::stoll(tokens[j + dj + 3].literal);
                        dj += 1;
                        break;
                    default:
                        break;
                    }

                    j += dj + 3;
                } else if (args_n >= inst_info.min_args()) {
                    j += 1;
                    break;
                } else
                    throw std::runtime_error("wrong instruction definition");

                inst_builder.add_argument(std::move(on_add));
                args_n++;

                // to add colon to diff
                if (tokens[j].type != TokenType::Semicolon)
                    j++;
                else
                    flag = false;
            }

            di = j - i + 1;
        }

        func_builder.add_line(inst_builder.get_product());
        return di;
    }
    size_t finish_parse(Context& ctx, size_t i, const token_vector& tokens, parse_queue& queue) {
        auto& [name, variant] = queue.front();
        auto scopes = ctx.get_proxy();

        if (name == "datatype_builder") {
            auto& builder = std::get<DatatypeBuilder>(variant);
            scopes.add(std::move(builder.get_product()));
        } else if (name == "function_builder") {
            auto& builder = std::get<FunctionBuilder>(variant);
            scopes.add(std::move(builder.get_product()));
        }

        queue.pop();

        return 1;
    }

    size_t do_parse_logic(Context& ctx, size_t i, const token_vector& tokens, parse_queue& queue) {
        using enum TokenType;

        static const std::unordered_map logic = {
            std::make_pair(KwStruct, start_parse_datatype),
            std::make_pair(KwFunc, start_parse_function),
            std::make_pair(KwVar, parse_variable),
            std::make_pair(InstName, parse_instruction),
            std::make_pair(RBrace, finish_parse)
        };

        size_t delta;
        if (const auto it = logic.find(tokens[i].type); it != logic.end())
            delta = it->second(ctx, i, tokens, queue);
        else
            throw std::runtime_error("this parse rule doesn't exist");

        return delta;
    }

    // Parser

    Parser::Parser(Context& ctx) :
        _ctx(ctx) {
    }

    ScopeContainer Parser::parse(const token_vector& tokens) const {
        parse_queue queue;

        for (size_t i = 0, di; i < tokens.size(); i += di)
            di = do_parse_logic(_ctx, i, tokens, queue);

        return _ctx.release_container();
    }
}
