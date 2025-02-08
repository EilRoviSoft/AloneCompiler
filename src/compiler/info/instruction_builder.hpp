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
        InstInfoBuilder& set_name(std::string name);
        InstInfoBuilder& set_code(lib::inst_code code);
        InstInfoBuilder& set_arguments_count(size_t count);
        InstInfoBuilder& set_arguments_count(size_t min, size_t max);
        InstInfoBuilder& set_bit_depth(size_t bit_depth);

        InstInfo&& get_product() override;

    private:
        struct {
            bool name : 1 = false;
            bool code : 1 = false;
        } _is_set;
    };

    class InstDeclBuilder : public lib::IBuilder<InstDecl> {
    public:
        InstDeclBuilder& set_info(const InstInfo& info);
        InstDeclBuilder& add_argument(argument_info&& arg);

        InstDecl&& get_product() override;

    private:
        struct {
            bool name : 1 = false;
        } _is_set;
    };
}
