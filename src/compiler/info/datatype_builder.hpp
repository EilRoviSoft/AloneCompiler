#pragma once

//library
#include "library/builder.hpp"

//compiler_info
#include "compiler/info/datatype.hpp"

namespace amasm::compiler {
	class DatatypeBuilder : public lib::IBuilder<Datatype> {
	public:
		DatatypeBuilder& set_name(std::string name);
		DatatypeBuilder& set_size(size_t size);
		DatatypeBuilder& add_pole(std::string name, const Datatype& type);

		Datatype&& get_product() override;

	private:
		struct {
			bool name : 1 = false;
			bool size : 1 = false;
		} _is_set;
	};
}
