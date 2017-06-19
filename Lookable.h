#ifndef LOOKABLE_H
#define LOOKABLE_H

#include <string>
#include <algorithm>
#include <unordered_set>

struct CaseInsensitiveCompare {
	static bool case_insensitive_char_cmp(char a, char b) {
		return tolower(a) == tolower(b);
	}

	bool operator() (const std::string& left, const std::string& right) const {
		if (left.size() == right.size()) {
			return std::equal(left.begin(), left.end(), right.begin(), case_insensitive_char_cmp);
		}

		return false;
	}
};

class Lookable {
	private:
		std::unordered_set<std::string, std::hash<std::string>, CaseInsensitiveCompare> aliases;

	public:
		std::string name;
		std::string look_string;

		void init_aliases() { aliases.insert(name); }
		bool matches_alias(std::string cmp) { return aliases.find(cmp) != aliases.end(); }
};

#endif