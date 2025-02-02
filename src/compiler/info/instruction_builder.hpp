#pragma once

//std
#include <string>

//library
#include "library/builder.hpp"
#include "library/types.hpp"

//compiler_info
#include "compiler/info/instruction.hpp"

namespace amasm::compiler {
    class InstInfoBuilder : public lib::IBuilder<InstInfo> {
    public:
        InstInfoBuilder& name(std::string name);
        InstInfoBuilder& code(lib::inst_code code);
        InstInfoBuilder& arguments_count(size_t count);
        InstInfoBuilder& arguments_count(size_t min, size_t max);
        InstInfoBuilder& bit_depth(size_t bit_depth);

        InstInfo&& build() override;

    private:
        struct {
            bool name      : 1 = false;
            bool code      : 1 = false;
            bool arguments : 1 = false;
            bool bit_depth : 1 = false;
        } _status;
    };

    class ArgumentInfoBuilder : public lib::IBuilder<ArgumentInfo> {
    public:
        ArgumentInfoBuilder& name(std::string name);
        ArgumentInfoBuilder& value(ptrdiff_t value);
        ArgumentInfoBuilder& type(ArgumentType type);

        ArgumentInfo&& build() override;

    private:
        struct {
            bool name  : 1 = false;
            bool value : 1 = false;
            bool type  : 1 = false;
        } _status;
    };

    class InstDeclBuilder : public lib::IBuilder<InstDecl> {
    public:
        InstDeclBuilder& name(std::string name);
        InstDeclBuilder& add_argument(ArgumentInfo&& arg);

        InstDecl&& build() override;

    private:
        struct {
            bool name : 1 = false;
        } _status;
    };
}
