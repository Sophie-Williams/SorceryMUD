#ifndef LOOKABLE_H
#define LOOKABLE_H

#include <string>
#include <algorithm>
#include <unordered_set>

#include <iostream>

class Lookable {
	private:
		std::unordered_set<std::string> aliases;

	public:
		std::string name;
		std::string look_string;

		void init_aliases() {
			// Temporary fix to remedy the fact that aliases with uppercase letters don't work
			std::string name_alias = name;
			std::transform(name_alias.begin(), name_alias.end(), name_alias.begin(), ::tolower);

			aliases.insert(name_alias);
		}
		bool matches_alias(std::string cmp) {
			// It is not currently necessary to convert the string to lowercase, because that's already done in Server::handle_req
			//std::transform(cmp.begin(), cmp.end(), cmp.begin(), ::tolower);
			return aliases.find(cmp) != aliases.end();
		}
};

#endif
