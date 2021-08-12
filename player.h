/*
* Justin W Li
* player.h
* Player class definition and function implementations
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

//--------------------
//----PLAYER CLASS----
//--------------------

class player {
	int hp;
	int atk;
	int exp;
	int level;

public:
	player() : hp(50), atk(5), exp(0), level(1) {}

	void gain_exp(int e) { exp += e; }
	bool can_level() const { return (exp > level * 10); }
	void level_up() {
		fully_heal();

		//boost stats
		++level;
		hp += 10;
		atk += 5;
		exp = 0;
	}

	int health() const { return hp; }
	int attack() const { return atk; }
	void defend(int dmg) { hp -= dmg; }
	bool alive() const { return hp > 0; }
	void fully_heal() { hp = (static_cast<int>(level) - 1) * 25 + 50; }	//fully heal player
	void game_over() { hp = 0; }						//sets hp to zero -- only called in player_quit
	void print_stats() { std::cout << "HP: " << hp << ", ATT: " 
		<< atk << ", EXP: " << exp << ", LVL: " << level << std::endl; }
};

#endif
