#pragma once

//library
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/constant.hpp"
#include "compiler/info/datatype.hpp"

namespace amasm::compiler {
	class ConstantBuilder : public lib::IBuilder<Constant> {
	public:
		ConstantBuilder& set_name(std::string name);
		ConstantBuilder& set_datatype(const Datatype& datatype);
		ConstantBuilder& set_data(lib::byte_vector&& data);

		bool is_built() const override;

		Constant&& get_product() override;

	private:
		struct {
			bool name     : 1 = false;
			bool datatype : 1 = false;
			bool data     : 1 = false;
		} _is_set;
	};
}
