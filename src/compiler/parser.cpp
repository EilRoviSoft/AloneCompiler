#include "compiler/parser.hpp"

//std
#include <optional>
#include <ranges>
#include <stdexcept>
#include <tuple>

//compiler_info
#include "compiler/info/datatype_builder.hpp"
#include "compiler/info/function_builder.hpp"
#include "compiler/info/instruction.hpp"
#include "compiler/info/instruction_builder.hpp"
#include "compiler/info/variable_builder.hpp"

namespace amasm::compiler::parser {
    using enum TokenType;

    // structs

    struct local_data {
        Context& ctx;
        size_t current_scope_id;
        ScopeProxy scopes;
        const token_vector& tokens;
    };

    // const data

    static const std::unordered_map<std::string, std::vector<TokenType>> rules = {
        { "struct_define", { KwStruct, Identifier, LBrace } },
        { "var_define", { KwVar, Percent, Identifier, Colon, Identifier } },
        { "func_define", { KwFunc, At, Identifier, LParen } },
        { "label_define", { KwLabel, Identifier, Colon } },
        { "direct_argument", { Percent, Identifier } },
        { "indirect_argument", { LBracket, Percent, Identifier } }
    };

    // utility functions

    bool match(const std::string& rule_name, const token_vector& tokens, size_t begin) {
        bool result = true;
        const auto& rule = rules.at(rule_name);

        for (size_t i = 0; result && i < rule.size(); i++)
            result = rule[i] == tokens[i + begin].type;

        return result;
    }
    std::tuple<ptrdiff_t, size_t> calc_offset(const Datatype& type, const token_vector& tokens, size_t begin) {
        const Datatype* curr_type = &type;
        ptrdiff_t offset = 0;
        size_t i = 0;

        while (tokens[i + begin].type == Dot) {
            const auto& pole = curr_type->pole(tokens[i + begin + 2].literal);
            curr_type = &pole.datatype();
            offset += pole.offset();
            i += 2;
        }

        return { offset, i };
    }

    // parsing functions

    // TODO: custom offset
    std::tuple<size_t, Pole> parse_pole_decl(size_t i, local_data& data) {
        if (!match("var_define", data.tokens, i))
            throw std::runtime_error("wrong pole definition");

        return {
            6,
            PoleBuilder()
            .set_name(data.tokens[i + 2].literal)
            .set_datatype(data.scopes.get_datatype(data.tokens[i + 4].literal))
            .get_product()
        };
    }
    std::tuple<size_t, Variable, std::optional<InstDecl>> parse_variable(size_t i, local_data& data) {
        if (!match("var_define", data.tokens, i))
            throw std::runtime_error("wrong variable definition");

        size_t delta = 6;
        VariableBuilder var_builder;
        InstDeclBuilder inst_builder;
        const auto& datatype = data.scopes.get_datatype(data.tokens[i + 4].literal);

        var_builder.set_name(data.tokens[i + 2].literal)
            .set_datatype(datatype);
        if (data.tokens[i + 5].type == Comma) {
            if (!match("indirect_argument", data.tokens, i + 6))
                throw std::runtime_error("wrong variable offset definition");

            ptrdiff_t offset = std::stoll(data.tokens[i + 10].literal);
            if (data.tokens[i + 9].type == Minus)
                offset *= -1;

            var_builder.set_relative_address(data.tokens[i + 8].literal, offset);

            delta += 7;
        } else if (data.current_scope_id != 0) {
            inst_builder.set_info(data.ctx.get_inst("push" + std::to_string(datatype.size() * 8)));
            var_builder.set_relative_address(data.tokens[i + 8].literal, -datatype.size());
        } else
            throw std::runtime_error("you cannot allocate variable inside global scope");

        return {
            delta,
            var_builder.get_product(),
            inst_builder.is_built() ? std::optional(inst_builder.get_product()) : std::nullopt
        };
    }

    size_t parse_datatype(size_t i, local_data& data) {
        if (!match("struct_define", data.tokens, i))
            throw std::runtime_error("wrong struct definition");

        size_t j = i + 3;
        auto builder = DatatypeBuilder();

        builder.set_name(data.tokens[i + 1].literal);
        while (data.tokens[j].type != RBrace) {
            auto [delta, pole] = parse_pole_decl(j, data);
            builder.add_pole(std::move(pole));
            j += delta;
        }

        data.scopes.add(builder.get_product());

        return j - i + 1;
    }

    std::tuple<size_t, InstDecl> parse_ncall(size_t i, const InstInfo& info, local_data& data) {
        size_t j = i + 1;
        InstDeclBuilder builder;
        std::string name;

        builder.set_info(info);
        while (data.tokens[j].type != Semicolon) {
            name += data.tokens[j].literal;
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
    std::tuple<size_t, InstDecl> parse_fcall(size_t i, const InstInfo& info, local_data& data) {
        size_t j = i + 1;
        InstDeclBuilder builder;
        std::string name;

        builder.set_info(info);
        while (data.tokens[j].type != Semicolon) {
            name += data.tokens[j].literal;
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
    std::tuple<size_t, InstDecl> parse_inst(size_t i, const InstInfo& info, local_data& data) {
        size_t j = i + 1, args_n = 0;
        bool flag = data.tokens[j].type != Semicolon;
        InstDeclBuilder builder;

        builder.set_info(info);
        while (args_n < info.max_args() && flag) {
            address_info on_add;

            if (parser::match("direct_argument", data.tokens, j)) {
                const auto& var = data.scopes.get_variable(data.tokens[j + 1].literal, data.current_scope_id);
                std::string true_var_name;
                auto [var_offset, dj] = parser::calc_offset(var.datatype(), data.tokens, j + 2);

                if (var.address().type == AddressType::RelativeWithDiff) {
                    true_var_name = var.address().name;
                    var_offset += var.address().sign_value;
                } else
                    true_var_name = data.tokens[j + 1].literal;

                on_add = {
                    .name = std::move(true_var_name),
                    .abs_value = (size_t) var_offset,
                    .sign_value = var_offset,
                    .type = var_offset ? AddressType::RelativeWithDiff : AddressType::Relative
                };
                j += dj + 2;
            } else if (data.tokens[j].type == Number) {
                on_add = {
                    .name = "",
                    .abs_value = std::stoull(data.tokens[j].literal),
                    .sign_value = 0,
                    .type = AddressType::Fixed
                };
                j++;
            } else if (parser::match("indirect_argument", data.tokens, j)) {
                const auto& var = data.scopes.get_variable(data.tokens[j + 2].literal, data.current_scope_id);
                std::string true_var_name;
                auto [var_offset, dj] = parser::calc_offset(var.datatype(), data.tokens, j + 3);

                if (var.address().type == AddressType::RelativeWithDiff) {
                    true_var_name = var.address().name;
                    var_offset += var.address().sign_value;
                } else
                    true_var_name = data.tokens[j + 2].literal;

                on_add = {
                    .name = std::move(true_var_name),
                    .abs_value = 0,
                    .sign_value = var_offset,
                    .type = AddressType::RelativeWithDiff
                };

                switch (data.tokens[j + dj + 3].type) {
                case Plus:
                    on_add.sign_value += std::stoll(data.tokens[j + dj + 4].literal);
                    dj += 1;
                    break;
                case Minus:
                    on_add.sign_value -= std::stoll(data.tokens[j + dj + 4].literal);
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
            if (data.tokens[j].type != Semicolon)
                j++;
            else
                flag = false;
        }

        return {
            j - i + 1,
            builder.get_product()
        };
    }
    std::tuple<size_t, std::string> parse_label(size_t i, local_data& data) {
        if (!parser::match("label_define", data.tokens, i))
            throw std::runtime_error("wrong variable definition");

        return {
            3,
            data.tokens[i + 1].literal
        };
    }

    size_t parse_function(size_t i, local_data& data) {
        if (!match("func_define", data.tokens, i))
            throw std::runtime_error("wrong func definition");

        size_t j;
        FunctionBuilder builder;
        data.current_scope_id = data.scopes.amount();

        builder.set_name(data.tokens[i + 2].literal)
            .set_scope(data.scopes, data.current_scope_id);

        // arguments dispatching up to rparen token
        for (j = i + 4; data.tokens[j].type != RParen; j++) {
            if ((j - i) % 2) {
                if (data.tokens[j].type != Comma)
                    throw std::runtime_error("wrong func definition");
            } else if (data.tokens[j].type == Identifier) {
                builder.add_argument_type(data.scopes.get_datatype(data.tokens[j].literal));
            } else
                throw std::runtime_error("wrong func definition");
        }

        // return-type detecting ('cause it's optional)
        if (data.tokens[j + 1].type == Colon) {
            builder.set_return_type(data.scopes.get_datatype(data.tokens[j + 2].literal));
            j += 4;
        } else {
            builder.set_return_type(data.scopes.get_datatype("void"));
            j += 2;
        }

        // dispatching lines
        while (data.tokens[j].type != RBrace) {
            size_t delta;

            if (data.ctx.has_inst(data.tokens[j].literal)) {
                InstDecl decl;
                const InstInfo& info = data.ctx.get_inst(data.tokens[j].literal);

                if (info.name() == "ncall")
                    std::tie(delta, decl) = parse_ncall(j, info, data);
                else if (info.name() == "fcall")
                    std::tie(delta, decl) = parse_fcall(j, info, data);
                else
                    std::tie(delta, decl) = parse_inst(j, info, data);

                builder.add_line(LineVariantBuilder()
                    .set_inst(std::move(decl))
                    .get_product());
            } else if (data.tokens[j].type == KwVar) {
                Variable var;
                std::optional<InstDecl> opt_decl;

                std::tie(delta, var, opt_decl) = parse_variable(j, data);

                data.scopes.add(std::move(var), data.current_scope_id);
                if (opt_decl.has_value())
                    builder.add_line(LineVariantBuilder()
                        .set_inst(std::move(opt_decl.value()))
                        .get_product());
            } else if (data.tokens[j].type == KwLabel) {
                std::string label;

                std::tie(delta, label) = parse_label(j, data);

                builder.add_line(LineVariantBuilder()
                    .set_label(std::move(label))
                    .get_product());
            } else
                throw std::invalid_argument("expected instruction, variable or label");

            j += delta;
        }

        data.scopes.add(builder.get_product());
        // change current scope to global if variable or const data will be occured
        data.current_scope_id = 0;

        return j - i + 1;
    }

    size_t do_logic(size_t i, local_data& data) {
        switch (data.tokens[i].type) {
        case KwStruct:
            return parse_datatype(i, data);
        case KwFunc:
            return parse_function(i, data);
        default:
            throw std::runtime_error("this parse rule doesn't exist");
        }
    }
}

namespace amasm::compiler {
    // public

    ScopeContainer Parser::parse(Context& ctx, token_vector tokens) {
        // preparing data for instructions names
        static auto is_inst = [&](const token& item) { return ctx.has_inst(item.literal); };
        for (auto& inst : tokens | std::views::filter(is_inst))
            inst.type = TokenType::InstName;

        parser::local_data data = {
            .ctx = ctx,
            .current_scope_id = 0,
            .scopes = ctx.get_proxy(),
            .tokens = tokens
        };

        for (size_t i = 0, di; i < data.tokens.size(); i += di)
            di = do_logic(i, data);

        return ctx.release_container();
    }
}
