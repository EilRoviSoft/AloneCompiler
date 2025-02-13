#include "compiler/parser.hpp"

//std
#include <ranges>
#include <stdexcept>
#include <tuple>

//compiler_info
#include "compiler/info/datatype_builder.hpp"
#include "compiler/info/function_builder.hpp"
#include "compiler/info/instruction_builder.hpp"
#include "compiler/info/variable_builder.hpp"

namespace amasm::compiler::parser {
    using enum TokenType;

    static const std::unordered_map<std::string, std::vector<TokenType>> rules = {
        { "struct_define", { KwStruct, Identifier, LBrace } },
        { "var_define", { KwVar, Percent, Identifier, Colon, Identifier } },
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
            curr_type = &pole.datatype();
            offset += pole.offset();
            i += 2;
        }

        return std::make_tuple(offset, i);
    }
}

namespace amasm::compiler {
    // public

    ScopeContainer Parser::parse(Context& ctx, const token_vector& tokens) {
        _ctx = &ctx;
        _tokens = tokens;
        _scopes = _ctx->get_proxy();

        // preparing data for instructions names
        static auto is_inst = [this](const token& item) { return _ctx->has_inst(item.literal); };
        for (auto& inst : _tokens | std::views::filter(is_inst))
            inst.type = TokenType::InstName;

        for (size_t i = 0, di; i < _tokens.size(); i += di)
            di = _do_parse_logic(i);

        return _ctx->release_container();
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

    size_t Parser::_parse_datatype(size_t i) {
        if (!parser::match("struct_define", _tokens, i))
            throw std::runtime_error("wrong struct definition");

        size_t j = i + 3;
        auto builder = DatatypeBuilder();

        builder.set_name(_tokens[i + 1].literal);
        while (_tokens[j].type != TokenType::RBrace) {
            auto [delta, pole] = _parse_pole_decl(j);
            builder.add_pole(std::move(pole));
            j += delta;
        }

        _scopes.add(builder.get_product());

        return j - i + 1;
    }

    size_t Parser::_parse_function(size_t i) {
        if (!parser::match("func_define", _tokens, i))
            throw std::runtime_error("wrong func definition");

        size_t j;
        FunctionBuilder builder;
        _current_scope_id = _scopes.amount();

        builder.set_name(_tokens[i + 2].literal)
            .set_scope(_scopes, _current_scope_id);

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

            if (_ctx->has_inst(_tokens[j].literal)) {
                InstDecl decl;
                const InstInfo& info = _ctx->get_inst(_tokens[j].literal);

                if (info.name() == "ncall")
                    std::tie(delta, decl) = _parse_ncall(j, info);
                else if (info.name() == "fcall")
                    std::tie(delta, decl) = _parse_fcall(j, info);
                else
                    std::tie(delta, decl) = _parse_inst(j, info);

                builder.add_line(std::move(decl));
            } else if (_tokens[j].type == TokenType::KwVar) {
                Variable var;
                std::optional<InstDecl> opt_decl;

                std::tie(delta, var, opt_decl) = _parse_variable(j);

                _scopes.add(std::move(var), _current_scope_id);
                if (opt_decl.has_value())
                    builder.add_line(std::move(opt_decl.value()));
            } else
                throw std::invalid_argument("expected instruction or variable");

            j += delta;
        }

        _scopes.add(builder.get_product());
        // change current scope to global if variable or const data will be occured
        _current_scope_id = 0;

        return j - i + 1;
    }

    std::tuple<size_t, InstDecl> Parser::_parse_ncall(size_t i, const InstInfo& info) {
        size_t j = i + 1;
        InstDeclBuilder builder;
        std::string name;

        builder.set_info(info);
        while (_tokens[j].type != TokenType::Semicolon) {
            name += _tokens[j].literal;
            j++;
        }
        builder.add_argument({
            .name = "",
            .abs_value = lib::hash_string(name),
            .sign_value = 0,
            .type = AddressType::Fixed
        });

        return {
            j - i + 1,
            builder.get_product()
        };
    }
    std::tuple<size_t, InstDecl> Parser::_parse_fcall(size_t i, const InstInfo& info) {
        size_t j = i + 1;
        InstDeclBuilder builder;
        std::string name;

        builder.set_info(info);
        while (_tokens[j].type != TokenType::Semicolon) {
            name += _tokens[j].literal;
            j++;
        }
        builder.add_argument({
            .name = name,
            .abs_value = 0,
            .sign_value = 0,
            .type = AddressType::Fixed
        });

        return {
            j - i + 1,
            builder.get_product()
        };
    }
    std::tuple<size_t, InstDecl> Parser::_parse_inst(size_t i, const InstInfo& info) {
        size_t j = i + 1, args_n = 0;
        bool flag = _tokens[j].type != TokenType::Semicolon;
        InstDeclBuilder builder;

        builder.set_info(info);
        while (args_n < info.max_args() && flag) {
            address_info on_add;

            if (parser::match("direct_argument", _tokens, j)) {
                const auto& var = _scopes.get_variable(_tokens[j + 1].literal, _current_scope_id);
                std::string true_var_name;
                auto [var_offset, dj] = parser::calc_offset(var.datatype(), _tokens, j + 2);

                if (var.address().type == AddressType::RelativeWithDiff) {
                    true_var_name = var.address().name;
                    var_offset += var.address().sign_value;
                } else
                    true_var_name = _tokens[j + 1].literal;

                on_add = {
                    .name = std::move(true_var_name),
                    .abs_value = (size_t) var_offset,
                    .sign_value = var_offset,
                    .type = var_offset ? AddressType::RelativeWithDiff : AddressType::Relative
                };
                j += dj + 2;
            } else if (_tokens[j].type == TokenType::Number) {
                on_add = {
                    .name = "",
                    .abs_value = std::stoull(_tokens[j].literal),
                    .sign_value = 0,
                    .type = AddressType::Fixed
                };
                j++;
            } else if (parser::match("indirect_argument", _tokens, j)) {
                const auto& var = _scopes.get_variable(_tokens[j + 2].literal, _current_scope_id);
                std::string true_var_name;
                auto [var_offset, dj] = parser::calc_offset(var.datatype(), _tokens, j + 3);

                if (var.address().type == AddressType::RelativeWithDiff) {
                    true_var_name = var.address().name;
                    var_offset += var.address().sign_value;
                } else
                    true_var_name = _tokens[j + 2].literal;

                on_add = {
                    .name = std::move(true_var_name),
                    .abs_value = 0,
                    .sign_value = var_offset,
                    .type = AddressType::RelativeWithDiff
                };

                switch (_tokens[j + dj + 3].type) {
                case TokenType::Plus:
                    on_add.sign_value += std::stoll(_tokens[j + dj + 4].literal);
                    dj += 1;
                    break;
                case TokenType::Minus:
                    on_add.sign_value -= std::stoll(_tokens[j + dj + 4].literal);
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
            j - i + 1,
            builder.get_product()
        };
    }

    // TODO: custom offset
    std::tuple<size_t, Pole> Parser::_parse_pole_decl(size_t i) {
        if (!parser::match("var_define", _tokens, i))
            throw std::runtime_error("wrong pole definition");

        return {
            6,
            PoleBuilder()
            .set_name(_tokens[i + 2].literal)
            .set_datatype(_scopes.get_datatype(_tokens[i + 4].literal))
            .get_product()
        };
    }
    std::tuple<size_t, Variable, std::optional<InstDecl>> Parser::_parse_variable(size_t i) {
        if (!parser::match("var_define", _tokens, i))
            throw std::runtime_error("wrong variable definition");

        size_t delta = 6;
        VariableBuilder var_builder;
        InstDeclBuilder inst_builder;
        const auto& datatype = _scopes.get_datatype(_tokens[i + 4].literal);

        var_builder.set_name(_tokens[i + 2].literal)
            .set_datatype(datatype);
        if (_tokens[i + 5].type == TokenType::Comma) {
            if (!parser::match("indirect_argument", _tokens, i + 6))
                throw std::runtime_error("wrong variable offset definition");

            ptrdiff_t offset = std::stoll(_tokens[i + 10].literal);
            if (_tokens[i + 9].type == TokenType::Minus)
                offset *= -1;

            var_builder.set_relative_address(_tokens[i + 8].literal, offset);

            delta += 7;
        } else if (_current_scope_id != 0) {
            inst_builder.set_info(_ctx->get_inst("push" + std::to_string(datatype.size() * 8)));
            var_builder.set_relative_address(_tokens[i + 8].literal, -datatype.size());
        } else
            throw std::runtime_error("you cannot allocate variable inside global scope");

        return {
            delta,
            var_builder.get_product(),
            inst_builder.is_built() ? std::optional(inst_builder.get_product()) : std::nullopt
        };
    }
}
