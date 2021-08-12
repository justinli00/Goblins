/*
* Justin W Li
* enemy_handler.h
* Enemy struct definition, handler class definition
*/

#ifndef ENEMY_HANDLER_H
#define ENEMY_HANDLER_H

#include <iostream>   //std::cout
#include <list>       //std::list
#include <cstdlib>    //rand, srand
#include <ctime>      //time
#include <string>     //string

//---------------------------
//----ENEMY HANDLER CLASS----
//---------------------------
class enemy_handler {

	//enemy class
	struct enemy {

		std::string name;           //name of type of enemy
		unsigned int type;			//type of enemy
		int hp;						//current health of enemy
		int atk;					//attack of enemy
		int exp;			//experience points dropped when killed

		//default ctor
		enemy(unsigned int type_);
	};

	//functor for operator<<
	struct print {
		void operator()(const enemy& e) {
			std::cout << e.name << "- Stats: " << e.atk << "/" << e.hp << "/" << e.exp << std::endl;
		}
	};

	std::list<enemy> enemies;							//enemy storage
	unsigned stage;										//game stage
	int threshholds[5];									//probability threshholds to spawn each enemy -- used in 

public:

	//names of each enemy difficulty/type
	enum difficulty { GOBLIN, GOB_SHAMAN, HOBGOBLIN, GOB_LORD, GOB_PALADIN };


	//ctor
	enemy_handler();

	//combat	
	int hp() const;								//returns hp of current enemy
	int hp_back() const;						//returns hp of enemy at back of line
	int attack() const;							//returns attack of current enemy
	int attack_back() const;					//returns attack of enemy at back of line
	void defend(int dmg);						//subtracts inputted damage from current enemy's health
	int exp() const;					//returns exp from killing current enemy
	unsigned int get_type() const;				//gets enemy type

	//enemy rotation/management
	void set_stage(const int performance);		//sets stage value based on performance
	void set_thresholds();						//calculates threshholds based on stage
	void spawn(); 								//spawns an enemy based on threshholds
	bool alive() const;							//checks if enemy at front of list is alive
	void die();									//pops enemy at front of list
	bool empty() const;							//returns bool regarding if list is empty or not
	void print_enemies() const;					//prints out counts of all enemies
	void kill_all();							//clears enemy queue
	std::string curr_name() const;				//gets name of enemy at front of list
	std::string last_name() const;				//gets name of enemy at end of list
	int enemies_left() const;					//returns number of enemies left in list
};

#endif