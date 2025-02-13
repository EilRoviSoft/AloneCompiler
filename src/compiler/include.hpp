#pragma once

//std
#include <memory>

//library
#include "library/bytecode.hpp"

//compiler
#include "compiler/context.hpp"
#include "compiler/lexer.hpp"
#include "compiler/parser.hpp"
#include "compiler/scanner.hpp"
#include "compiler/translator.hpp"

namespace amasm {
    class Compiler {
    public:
        static lib::Bytecode process_from(const std::string& filename) {
            const auto text = compiler::Scanner::scan_from_file(filename);
            return process(text);
        }
        static lib::Bytecode process(const std::string& text) {
            CompilerContext ctx;
            const auto tokens = compiler::Lexer::tokenize_code(text);
            const auto composed_data = compiler::Parser::parse(ctx, tokens);
            return compiler::Translator::translate(composed_data);
        }
    };
}
