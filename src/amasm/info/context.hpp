#pragma once

//std
#include <string>
#include <unordered_map>
#include <unordered_set>

//amasm
#include "amasm/info/datatypes.hpp"
#include "amasm/info/tokens.hpp"

namespace alone::amasm {
	class AmasmContext {
	public:
		void init();

		void insert_datatype(const datatype_ptr& ptr);

		const token_type& get_token_or(const std::string& key, const token_type& default_token) const;
		datatype_ptr get_datatype(const std::string& key) const;
		const std::vector<token_type>& get_rule(const std::string& key) const;

	private:
		std::unordered_map<std::string, token_type> _defined_tokens;
		std::unordered_map<size_t, datatype_ptr> _datatypes;
		std::unordered_map<std::string, std::vector<token_type>> _rules;
		std::unordered_set<std::string> _surrounded_by_braces_signatures;

		void _init_defined_tokens();
		void _init_datatypes();
		void _init_rules();
		void _init_surrounded_by_braces_signatures();
	};
}
