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
        static std::shared_ptr<CompilerContext> ctx;

        static void init() {
            ctx = std::make_shared<CompilerContext>();
        }

        static lib::Bytecode compile_from(const std::string& filename) {
            const auto text = compiler::Scanner::scan_from_file(filename);
            return compile(text);
        }
        static lib::Bytecode compile(const std::string& text) {
            const auto tokens = compiler::Lexer().tokenize_code(text);
            const auto composed_data = compiler::Parser().parse(*ctx, tokens);
            return compiler::Translator().translate(composed_data);
        }
    };
}
