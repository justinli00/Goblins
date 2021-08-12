/*
* Justin W Li
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
		unsigned int exp;			//experience points dropped when killed

		//default ctor
		enemy(int type_);
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

protected:
	//names of each enemy difficulty/type
	enum difficulty { GOBLIN, GOB_SHAMAN, HOBGOBLIN, GOB_LORD, GOB_PALADIN };

	//ctor
	enemy_handler();

	//combat	
	int hp();											//returns hp of current enemy
	int attack();										//returns attack of current enemy
	void defend(int dmg);								//subtracts inputted damage from current enemy's health
	unsigned int exp();									//returns exp from killing current enemy
	unsigned int kill_all();							//empties enemies, returns collective exp
	int get_type();										//gets enemy type

	//enemy rotation/management
	void set_stage(int performance);					//sets stage value based on performance
	void set_thresholds();								//calculates threshholds based on stage
	void spawn(); 										//spawns an enemy based on threshholds
	bool alive();										//checks if enemy at front of list is alive
	void die();											//pops enemy at front of list
	bool empty();										//returns bool regarding if list is empty or not
	void print_enemies();								//prints out counts of all enemies
	std::string curr_name();                            //gets name of enemy at front of list
	std::string last_name();                            //gets name of enemy at end of list
	int enemies_left();                                 //returns number of enemies left in list
};

#endif