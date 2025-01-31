#pragma once

//lib
#include "library/bytecode.hpp"

//compiler
#include "compiler/context.hpp"
#include "compiler/functions.hpp"

namespace amasm::compiler {
    class Translator {
    public:
        struct hint {
            size_t offset;
            std::string name;
        };

        explicit Translator(Context& ctx);

        lib::Bytecode translate(funcs_queue composed) const;

    private:
        Context& _ctx;

        std::tuple<lib::Bytecode, std::list<hint>> _decompose_instruction(const func_info& scope,
                                                                             const inst_decl& inst)
        const;
    };
}
