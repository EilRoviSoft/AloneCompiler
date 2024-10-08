#pragma once

//std
#include <stdexcept>

#define GENERAL_WRONG_WORD_VALUE std::runtime_error("general.hpp: wrong word value")
#define GENERAL_WRONG_FLOATING_VALUE std::runtime_error("general.hpp: wrong floating value")
#define GENERAL_WRONG_FLOATING_VALUE_AFTER_DOT std::runtime_error("general.hpp: wrong floating value after '.'")
#define GENERAL_WRONG_BINARY_VALUE std::runtime_error("general.hpp: wrong binary value")
#define GENERAL_WRONG_HEX_VALUE std::runtime_error("general.hpp: wrong hex value")
#define GENERAL_WRONG_DECIMAL_VALUE std::runtime_error("general.hpp: wrong decimal value")

#define AMASM_DATA_TYPES_EMPTY_STRUCT std::runtime_error("data_types.cpp: struct size should not be 0")
#define AMASM_DATA_TYPES_WRONG_CONSTRUCTOR_USAGE std::runtime_error("data_types.cpp: poles should not be empty, use another constructor instead")

#define AMASM_PARSE_RULES_WRONG_LITERAL std::runtime_error("data_types.cpp: wrong literal")

#define AMASM_TOKENS_WRONG_INPUT std::runtime_error("tokens.cpp: wrong input")

#define AMASM_LEXER_WRONG_LITERAL_TYPE std::runtime_error("lexer.cpp: wrong literal type")

#define AMASM_PARSER_WRONG_STRUCT_DEFINITION std::runtime_error("parser.cpp: wrong structure definition")
#define AMASM_PARSER_WRONG_POLE_DEFINITION std::runtime_error("parser.cpp: wrong pole definition")
#define AMASM_PARSER_WRONG_FUNC_DEFINITION std::runtime_error("parser.cpp: wrong function definition")

#define AMASM_PARSER_WRONG_POLE_DEFINITION_PLACE std::runtime_error("parser.cpp: pole is defined outside of struct")
#define AMASM_PARSER_WRONG_BRACE_PLACEMENT std::runtime_error("parser.cpp: wrong braces placement")
#define AMASM_PARSER_WRONG_FUNC_ARGS_VALUE std::runtime_error("parser.cpp: wrong function args value")
#define AMASM_PARSER_WRONG_FLAG_TYPE std::runtime_error("parser.cpp: wrong flag type")
#define AMASM_PARSER_WRONG_PARSE_RULE_TYPE std::runtime_error("parser.cpp: wrong parse rule type")
