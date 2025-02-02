#pragma once

//std
#include <list>
#include <string>
#include <unordered_map>

//compiler_info
#include "compiler/info/scope_info.hpp"

//library
#include "library/builder.hpp"

namespace amasm::compiler {
	class Datatype : public IScopeElement {
		friend class ScopeProxy;
		friend class DatatypeBuilder;

		struct pole_info {
			std::string name;
			const Datatype& type;
			ptrdiff_t offset;
		};

	public:
		Datatype(const Datatype&) = default;
		Datatype(Datatype&&) = default;

		size_t size() const;
		const pole_info& pole(const std::string& pole_name) const;

	protected:
		size_t m_size;
		std::list<pole_info> m_poles;
		std::unordered_map<size_t, const pole_info&> m_poles_search;

	private:
		Datatype() = default;
	};

	class DatatypeBuilder : public lib::IBuilder<Datatype> {
	public:
		DatatypeBuilder& name(std::string name);
		DatatypeBuilder& size(size_t size);
		DatatypeBuilder& add_pole(std::string name, const Datatype& type);

		Datatype&& build() override;

	private:
		struct {
			bool name : 1 = false;
			bool size : 1 = false;
		} _status;
	};
}
