#pragma once

//library
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/datatype.hpp"

namespace amasm::compiler {
    class PoleBuilder : public lib::IBuilder<Pole> {
    public:
        PoleBuilder& set_name(std::string name);
        PoleBuilder& set_datatype(const Datatype& datatype);

        bool is_built() const override;

        Pole&& get_product() override;

    private:
        struct {
            bool name     : 1 = false;
            bool datatype : 1 = false;
        } _is_set;
    };

    class DatatypeBuilder : public lib::IBuilder<Datatype> {
    public:
        DatatypeBuilder& set_name(std::string name);
        DatatypeBuilder& set_size(size_t size);
        DatatypeBuilder& add_pole(Pole&& pole);

        bool is_built() const override;

        Datatype&& get_product() override;

    private:
        struct {
            bool name : 1 = false;
        } _is_set;
    };
}
