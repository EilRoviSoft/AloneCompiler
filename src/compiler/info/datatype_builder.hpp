#pragma once

//library
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/datatype.hpp"

namespace amasm::compiler {
    class PoleDeclBuilder : public lib::IBuilder<PoleDecl> {
    public:
        PoleDeclBuilder& set_name(std::string name);
        PoleDeclBuilder& set_datatype(const Datatype& type);
        PoleDeclBuilder& set_offset(ptrdiff_t offset);

        PoleDecl&& get_product() override;

    private:
        struct {
            bool name : 1 = false;
            bool type : 1 = false;
        } _is_set;
    };

    class DatatypeBuilder : public lib::IBuilder<Datatype> {
    public:
        DatatypeBuilder& set_name(std::string name);
        DatatypeBuilder& set_size(size_t size);
        DatatypeBuilder& add_pole(std::string name, const Datatype& type);
        DatatypeBuilder& add_pole(PoleDecl&& pole);

        Datatype&& get_product() override;

    private:
        struct {
            bool name : 1 = false;
        } _is_set;
    };
}
