#include "parser.hpp"

//std
#include <string>
#include <vector>

//amasm
#include "amasm/arguments.hpp"
#include "amasm/data_types.hpp"
#include "amasm/variables.hpp"

namespace alone::amasm::inline parser_inlined {
    struct inst_t {
        std::string name;
        std::vector<argument_t> args;
    };
    struct scope_info_t {
        std::vector<inst_t> lines;
        //std::unordered_map<std::string, >
    };
}

namespace alone::amasm {
    Parser::Parser(const AmasmContext& amasm_ctx) :
        _amasm_context(amasm_ctx) {
    }

    shared::Bytecode Parser::parse(const token_array_t& tokens) {}
}
