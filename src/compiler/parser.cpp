#include "compiler/parser.hpp"

//std
#include <ranges>
#include <stdexcept>
#include <tuple>

//compiler_info
#include "compiler/info/datatype_builder.hpp"
#include "compiler/info/instruction_builder.hpp"
#include "compiler/info/variable_builder.hpp"

namespace amasm::compiler::parser {
    using enum TokenType;

    static const std::unordered_map<std::string, std::vector<TokenType>> rules = {
        { "struct_define", { KwStruct, Identifier, LBrace, Colon, Identifier } },
        { "pole_define", { KwVar, Percent, Identifier, Comma, Identifier } },
        { "func_define", { KwFunc, At, Identifier, LParen } },
        { "direct_argument", { Percent, Identifier } },
        { "indirect_argument", { LBracket, Percent, Identifier } }
    };

    auto match(const std::string& rule_name, const token_vector& tokens, size_t begin) {
        bool result = true;
        const auto& rule = rules.at(rule_name);

        for (size_t i = 0; result && i < rule.size(); i++)
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
}

namespace amasm::compiler {
    // public

    Parser::Parser(Context& ctx) :
        _ctx(ctx) {
    }

    ScopeContainer Parser::parse(token_vector tokens) {
        _tokens = std::move(tokens);
        _scopes = _ctx.get_proxy();

        // preparing data for instructions names
        static auto is_inst = [this](const token& item) { return _ctx.has_inst(item.literal); };
        for (auto& inst : _tokens | std::views::filter(is_inst))
            inst.type = TokenType::InstName;

        for (size_t i = 0, di; i < _tokens.size(); i += di)
            di = _do_parse_logic(i);

        return _ctx.release_container();
    }

    // private

    size_t Parser::_do_parse_logic(size_t i) {
        switch (_tokens[i].type) {
        case TokenType::KwStruct:
            return _parse_datatype(i);
        case TokenType::KwFunc:
            return _parse_function(i);
        default:
            throw std::runtime_error("this parse rule doesn't exist");
        }
    }

    // TODO
    size_t Parser::_parse_datatype(size_t i) {
        if (!parser::match("struct_define", _tokens, i))
            throw std::runtime_error("wrong struct definition");

        auto builder = DatatypeBuilder();
        builder.set_name(_tokens[i + 1].literal);

        return 3;
    }

    size_t Parser::_parse_function(size_t i) {
        if (!parser::match("func_define", _tokens, i))
            throw std::runtime_error("wrong func definition");

        size_t j;
        FunctionBuilder builder;

        builder.set_name(_tokens[i + 2].literal)
            .set_scope(_scopes, _scopes.amount());

        // arguments dispatching up to rparen token
        for (j = i + 4; _tokens[j].type != TokenType::RParen; j++) {
            if ((j - i) % 2) {
                if (_tokens[j].type != TokenType::Comma)
                    throw std::runtime_error("wrong func definition");
            } else if (_tokens[j].type == TokenType::Identifier) {
                builder.add_argument_type(_scopes.get_datatype(_tokens[j].literal));
            } else
                throw std::runtime_error("wrong func definition");
        }

        // return-type detecting ('cause it's optional)
        if (_tokens[j + 1].type == TokenType::Colon) {
            builder.set_return_type(_scopes.get_datatype(_tokens[j + 2].literal));
            j += 4;
        } else {
            builder.set_return_type(_scopes.get_datatype("void"));
            j += 2;
        }

        // dispatching lines
        while (_tokens[j].type != TokenType::RBrace) {
            size_t delta;
            InstDecl decl;
            const InstInfo& info = _ctx.get_inst(_tokens[j].literal);

            if (_tokens[j].literal == "fcall")
                std::tie(decl, delta) = _parse_fcall(j, info);
            else
                std::tie(decl, delta) = _parse_inst(j, info);

            builder.add_line(std::move(decl));
            j += delta;
        }

        _scopes.add(builder.get_product());

        return j - i + 1;
    }

    std::tuple<InstDecl, size_t> Parser::_parse_fcall(size_t i, const InstInfo& info) {
        size_t j = i + 1;
        InstDeclBuilder builder;
        argument_info on_add;

        builder.set_info(info);
        while (_tokens[j].type != TokenType::Semicolon) {
            on_add.name += _tokens[j].literal;
            j++;
        }
        on_add.type = ArgumentType::JumpAddress;
        builder.add_argument(std::move(on_add));

        return {
            builder.get_product(),
            j - i + 1
        };
    }
    std::tuple<InstDecl, size_t> Parser::_parse_inst(size_t i, const InstInfo& info) {
        size_t j = i + 1, args_n = 0, local_id = _scopes.amount() - 1;
        bool flag = _tokens[j].type != TokenType::Semicolon;
        InstDeclBuilder builder;

        builder.set_info(info);
        while (args_n < info.max_args() && flag) {
            argument_info on_add;

            if (parser::match("direct_argument", _tokens, j)) {
                const auto& var = _scopes.get_variable(_tokens[j + 1].literal, local_id);
                auto [var_offset, dj] = parser::calc_offset(var.datatype(), _tokens, j + 2);
                on_add = {
                    .name = _tokens[j + 1].literal,
                    .value = var_offset,
                    .type = var_offset ? ArgumentType::IndirectWithDisplacement : ArgumentType::Direct
                };
                j += dj + 2;
            } else if (_tokens[j].type == TokenType::Number) {
                on_add = {
                    .name = "",
                    .value = std::stoll(_tokens[j].literal),
                    .type = ArgumentType::Immediate
                };
                j++;
            } else if (parser::match("indirect_argument", _tokens, j)) {
                const auto& var = _scopes.get_variable(_tokens[j + 2].literal, local_id);
                auto [var_offset, dj] = parser::calc_offset(var.datatype(), _tokens, j + 3);
                on_add = {
                    .name = _tokens[j + 2].literal,
                    .value = var_offset,
                    .type = ArgumentType::IndirectWithDisplacement
                };

                switch (_tokens[j + dj + 3].type) {
                case TokenType::Plus:
                    on_add.value += std::stoll(_tokens[j + dj + 4].literal);
                    dj += 1;
                    break;
                case TokenType::Minus:
                    on_add.value -= std::stoll(_tokens[j + dj + 4].literal);
                    dj += 1;
                    break;
                default:
                    break;
                }

                j += dj + 4;
            } else if (args_n >= info.min_args()) {
                j += 1;
                break;
            } else
                throw std::runtime_error("wrong instruction definition");

            builder.add_argument(std::move(on_add));
            args_n++;

            // to add colon to diff
            if (_tokens[j].type != TokenType::Semicolon)
                j++;
            else
                flag = false;
        }

        return {
            builder.get_product(),
            j - i + 1
        };
    }

    std::tuple<Datatype, size_t> Parser::_parse_variable(size_t i) {
        if (!parser::match("pole_define", _tokens, i))
            throw std::runtime_error("wrong pole definition");

        DatatypeBuilder builder;
        bool has_own_offset = _tokens[i + 5].type == TokenType::Comma;

        builder.add_pole(_tokens[i + 2].literal, _scopes.get_datatype(_tokens[i + 4].literal));

        return {
            builder.get_product(),
            has_own_offset ? 13 : 6
        };
    }
}
