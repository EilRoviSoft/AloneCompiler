#include "parser.hpp"

//std
#include <span>

#define BIND_RULE(TYPE, PRED) std::make_pair(TYPE, std::function<size_t(const size_t&)>(std::bind(PRED, this, std::placeholders::_1)))
#define CHECK_RULE(RULE_NAME, MESSAGE) if (!match_rule(_ctx, RULE_NAME, std::span(_tokens.begin() + i, _ctx.get_rule(RULE_NAME).size()))) { throw std::runtime_error(MESSAGE); }

namespace alone::amasm::inline parser_inlined {
    bool match_rule(const AmasmContext& ctx, const std::string& name, const std::span<token_t>& tokens) {
        const auto& pattern = ctx.get_rule(name);
        bool result = true;

        for (size_t i = 0; result && i < pattern.size(); i++)
            result = pattern[i] == tokens[i].type;

        return result;
    }
    std::tuple<ptrdiff_t, size_t> calc_offset(const datatype_ptr& type, const std::span<token_t>& tokens) {
        datatype_ptr curr_type = type;
        ptrdiff_t offset = 0;
        size_t delta = 0;

        for (size_t i = 1; tokens[i].type != token_type::semicolon &&
             tokens[i].type == token_type::comma; i += 2, delta += 2) {
            const auto it = std::ranges::find_if(curr_type->poles, [&](const pole_t& pole) {
                return pole.name == tokens[i + 1].literal;
            });
            curr_type = it->type;
            offset += it->offset;
        }

        return { offset, delta };
    }
}

namespace alone::amasm {
    Parser::Parser(AmasmContext& ctx) :
        _ctx(ctx) {
    }

    shared::Bytecode Parser::parse(token_array_t tokens) {
        shared::Bytecode result;
        size_t summed_size = 8;

        _tokens = std::move(tokens);
        for (size_t i = 0, di = 0, address = 0; i < _tokens.size(); i += di, di = 0)
            di = _do_parse_logic(i);

        _tokens.clear();
        _labels.clear();

        return result;
    }

    size_t Parser::_do_parse_logic(const size_t& i) {
        using enum token_type;

        static const std::unordered_map logic = {
            BIND_RULE(kw_struct, _start_parse_struct),
            BIND_RULE(kw_func, _start_parse_func),
            BIND_RULE(kw_var, _parse_variable),
            BIND_RULE(inst_name, _parse_instruction),
            BIND_RULE(rbrace, _finish_parse)
        };

        size_t delta;
        if (const auto it = logic.find(_tokens[i].type); it != logic.end())
            delta = it->second(i);
        else
            throw std::runtime_error("this parse rule doesn't exist");

        return delta;
    }

    size_t Parser::_start_parse_struct(const size_t& i) {
        CHECK_RULE("struct_define", "wrong struct definition")

        datatype_ptr ntype = make_datatype(_tokens[i + 1].literal, 0);
        _queue.emplace("struct_define", false, std::move(ntype));

        return 3;
    }
    size_t Parser::_start_parse_func(const size_t& i) {
        CHECK_RULE("func_define", "wrong func definition")

        size_t j, delta;
        func_info_t on_push;

        on_push.name = _tokens[i + 2].literal;
        // arguments dispatching up to rparen token
        for (j = i + 4; _tokens[j].type != token_type::rparen; j++) {
            if ((j - i) % 2) {
                if (_tokens[j].type != token_type::comma)
                    throw std::runtime_error("wrong func definition");
            } else if (_tokens[j].type == token_type::datatype) {
                on_push.args.emplace_back(_ctx.get_datatype(_tokens[j].literal));
            } else
                throw std::runtime_error("wrong func definition");
        }
        on_push.args.shrink_to_fit();

        // return-type detecting ('cause it's optional)
        if (_tokens[j + 1].type == token_type::colon) {
            on_push.return_type = _ctx.get_datatype(_tokens[j + 2].literal);
            delta = j - i + 4;
        } else {
            on_push.return_type = _ctx.get_datatype("void");
            delta = j - i + 2;
        }
        _queue.emplace("func_define", false, std::move(on_push));

        return delta;
    }

    size_t Parser::_parse_variable(const size_t& i) {
        CHECK_RULE("pole_define", "wrong pole definition")

        size_t delta;

        const auto& [name, is_finished, data] = _queue.front();
        if (name == "struct_define") {
            const auto& ntype = std::get<datatype_ptr>(data);
            const bool has_own_offset = _tokens[i + 5].type == token_type::comma;

            ntype->add_pole(_tokens[i + 2].literal, _ctx.get_datatype(_tokens[i + 4].literal));
            delta = has_own_offset ? 13 : 6;
        } else
            throw std::runtime_error("isn't done yet");

        return delta;
    }

    size_t Parser::_parse_instruction(const size_t& i) {
        auto& [name, is_finished, data] = _queue.front();
        if (name != "func_define" || is_finished)
            throw std::runtime_error("wrong instruction definition placement");

        size_t j;
        auto& func_data = std::get<func_info_t>(data);
        //const auto& inst_info = ;
    }
}
