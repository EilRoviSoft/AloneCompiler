#include "compiler/parser.hpp"

//std
#include <functional>
#include <stdexcept>

//frozen
#include "frozen/string.h"
#include "frozen/unordered_map.h"

//compiler_info
#include "compiler/info/instruction_builder.hpp"
#include "compiler/info/variable_builder.hpp"

namespace amasm::compiler::parser {
    using enum TokenType;

    constinit const frozen::unordered_map<frozen::string, std::array<TokenType, 6>, 5> rules = {
        { "struct_define", { KwStruct, Identifier, LBrace, Max } },
        { "pole_define", { KwVar, Percent, Identifier, Comma, Identifier, Max } },
        { "func_define", { KwFunc, At, Identifier, LParen, Max } },
        { "direct_argument", { Percent, Identifier, Max } },
        { "indirect_argument", { LBracket, Percent, Identifier, Max } }
    };

    auto match(const frozen::string& rule_name, const token_vector& tokens, size_t begin) {
        bool result = true;
        const auto& rule = rules.at(rule_name);

        for (size_t i = 0; result && rule[i] != Max; i++)
            result = rule[i] == tokens[i + begin].type;

        return result;
    }
    auto calc_offset(const Datatype& type, const token_vector& tokens, size_t begin) {
        const Datatype* curr_type = &type;
        ptrdiff_t offset = 0;
        size_t i = 0;

        while (tokens[i + begin].type == Dot) {
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

    size_t start_parse_datatype(Context& ctx, size_t i, const token_vector& tokens, queue& queue) {
        if (!match("struct_define", tokens, i))
            throw std::runtime_error("wrong struct definition");

        queue.emplace(
            DatatypeDefinition,
            std::move(DatatypeBuilder().set_name(tokens[i + 1].literal))
        );

        return 3;
    }
    size_t start_parse_function(Context& ctx, size_t i, const token_vector& tokens, queue& queue) {
        if (!match("func_define", tokens, i))
            throw std::runtime_error("wrong func definition");

        size_t j, delta;
        FunctionBuilder builder;
        auto scopes = ctx.get_proxy();

        builder.set_name(tokens[i + 2].literal)
               .set_scope(scopes, scopes.amount());

        // arguments dispatching up to rparen token
        for (j = i + 4; tokens[j].type != RParen; j++) {
            if ((j - i) % 2) {
                if (tokens[j].type != Comma)
                    throw std::runtime_error("wrong func definition");
            } else if (tokens[j].type == Identifier) {
                builder.add_argument_type(scopes.get_datatype(tokens[j].literal));
            } else
                throw std::runtime_error("wrong func definition");
        }

        // return-type detecting ('cause it's optional)
        if (tokens[j + 1].type == Colon) {
            builder.set_return_type(scopes.get_datatype(tokens[j + 2].literal));
            delta = j - i + 4;
        } else {
            builder.set_return_type(scopes.get_datatype("void"));
            delta = j - i + 2;
        }

        queue.emplace(FunctionDefinition, std::move(builder));
        return delta;
    }
    size_t parse_variable(Context& ctx, size_t i, const token_vector& tokens, queue& queue) {
        if (!match("pole_define", tokens, i))
            throw std::runtime_error("wrong pole definition");

        size_t delta;
        auto& [name, data] = queue.front();
        auto scopes = ctx.get_proxy();

        if (name == DatatypeDefinition) {
            auto& builder = std::get<DatatypeBuilder>(data);
            bool has_own_offset = tokens[i + 5].type == TokenType::Comma;

            builder.add_pole(tokens[i + 2].literal, scopes.get_datatype(tokens[i + 4].literal));
            delta = has_own_offset ? 13 : 6;
        } else
            throw std::runtime_error("isn't done yet");

        return delta;
    }
    size_t parse_instruction(Context& ctx, size_t i, const token_vector& tokens, queue& queue) {
        auto& [name, data] = queue.front();
        if (name != FunctionDefinition)
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
            while (tokens[j].type != Semicolon) {
                on_add.name += tokens[j].literal;
                j++;
            }

            inst_builder.add_argument(std::move(on_add));
            di = j - i + 1;
        } else {
            size_t args_n = 0;
            bool flag = tokens[j].type != Semicolon;
            const auto& inst_info = ctx.get_inst_info(tokens[i].literal);

            while (args_n < inst_info.max_args() && flag) {
                argument_info on_add;

                if (match("direct_argument", tokens, j)) {
                    const auto& var = scopes.get_variable(tokens[j + 1].literal, scope_id);
                    auto [var_offset, dj] = calc_offset(var.datatype(), tokens, j + 2);
                    on_add = {
                        .name = tokens[j + 1].literal,
                        .value = var_offset,
                        .type = var_offset ? ArgumentType::IndirectWithDisplacement : ArgumentType::Direct
                    };
                    j += dj + 2;
                } else if (tokens[j].type == Number) {
                    on_add = {
                        .name = "",
                        .value = std::stoll(tokens[j].literal),
                        .type = ArgumentType::Immediate
                    };
                    j++;
                } else if (match("indirect_argument", tokens, j)) {
                    const auto& var = scopes.get_variable(tokens[j + 2].literal, scope_id);
                    auto [var_offset, dj] = calc_offset(var.datatype(), tokens, j + 3);
                    on_add = {
                        .name = tokens[j + 2].literal,
                        .value = var_offset,
                        .type = ArgumentType::IndirectWithDisplacement
                    };

                    switch (tokens[j + dj + 3].type) {
                    case Plus:
                        on_add.value += std::stoll(tokens[j + dj + 4].literal);
                        dj += 1;
                        break;
                    case Minus:
                        on_add.value -= std::stoll(tokens[j + dj + 4].literal);
                        dj += 1;
                        break;
                    default:
                        break;
                    }

                    j += dj + 4;
                } else if (args_n >= inst_info.min_args()) {
                    j += 1;
                    break;
                } else
                    throw std::runtime_error("wrong instruction definition");

                inst_builder.add_argument(std::move(on_add));
                args_n++;

                // to add colon to diff
                if (tokens[j].type != Semicolon)
                    j++;
                else
                    flag = false;
            }

            di = j - i + 1;
        }

        func_builder.add_line(inst_builder.get_product());
        return di;
    }
    size_t finish_parse(Context& ctx, size_t i, const token_vector& tokens, queue& queue) {
        auto& [name, variant] = queue.front();
        auto scopes = ctx.get_proxy();

        if (name == DatatypeDefinition) {
            auto& builder = std::get<DatatypeBuilder>(variant);
            scopes.add(std::move(builder.get_product()));
        } else if (name == FunctionDefinition) {
            auto& builder = std::get<FunctionBuilder>(variant);
            scopes.add(std::move(builder.get_product()));
        }

        queue.pop();

        return 1;
    }

    size_t do_parse_logic(Context& ctx, size_t i, const token_vector& tokens, queue& queue) {
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
}

namespace amasm::compiler {
    Parser::Parser(Context& ctx) :
        _ctx(ctx) {
    }

    ScopeContainer Parser::parse(const token_vector& tokens) const {
        parser::queue queue;

        for (size_t i = 0, di; i < tokens.size(); i += di)
            di = do_parse_logic(_ctx, i, tokens, queue);

        return _ctx.release_container();
    }
}
