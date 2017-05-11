#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

class Character {
	private:
		// I'm not sure if a charid attribute is necessary
		std::string name;
		char gender;
		std::string race;
		std::string game_class;
		int level;
		int xp;
		// attributes
		int loc;
		int hp_cur;
		int hp_max;
		int money;
		// inventory
		// abilities
		// spells
		// status conditions
		// owner -- is this necessary?
	
	public:
		Character() { }

		void set_name(std::string n) { name = n; }
		void set_gender(char g) { gender = g; }
		void set_race(std::string r) { race = r; }
		void set_class(std::string c) { game_class = c; }
		void set_level(int l) { level = l; };
		void set_xp(int x) { xp = x; }
		void set_loc(int l) { loc = l; }
		void set_hp_cur(int h) { hp_cur = h; }
		void set_hp_max(int h) { hp_max = h; }
		void set_money(int m) { money = m; }

		int get_loc() { return loc; }
};

#endif
