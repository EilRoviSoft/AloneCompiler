#pragma once

//std
#include <string>
#include <unordered_map>
#include <unordered_set>

//amasm
#include "amasm/info/datatypes.hpp"
#include "amasm/info/instructions.hpp"
#include "amasm/info/Tokens.hpp"
#include "amasm/info/variables.hpp"

namespace alone::amasm {
	class Context {
	public:
		Context();

		void insert_datatype(const datatype_ptr& ptr);

		const Tokens& get_token_or(const std::string& key, const Tokens& default_token) const;
		datatype_ptr get_datatype(const std::string& key) const;
		const std::vector<Tokens>& get_rule(const std::string& key) const;
		const inst_info_t& get_inst(const std::string& key);

	private:
		std::unordered_map<std::string, Tokens> _defined_tokens;
		std::unordered_map<size_t, datatype_ptr> _datatypes;
		std::unordered_map<std::string, std::vector<Tokens>> _rules;
		std::unordered_set<std::string> _surrounded_by_braces_signatures;
		std::unordered_map<size_t, inst_info_t> _instructions;
		Variables _global_variables;

		void _init();
		void _init_defined_tokens();
		void _init_datatypes();
		void _init_rules();
		void _init_surrounded_by_braces_signatures();
		void _init_instructions();
		void _init_global_variables();
	};
}
