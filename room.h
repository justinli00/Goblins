/*
*Justin W Li
*Room and room handler defintions
*/

#ifndef ROOM_HANDLER_H
#define ROOM_HANDLER_H

#include <list>				//std::list

//------------------
//----ROOM CLASS----
//------------------
//essentially tracks performance of player, adjusts difficulty as necessary
class room_handler
{
	struct room {
		unsigned int turns;					//turns needed to complete room

		//enemy spawns
		unsigned int spawned;				//number of enemies spawned
		unsigned int limit;					//maximum number of enemies to be spawned

		//combat
		unsigned int killed;				//number of enemies killed
		unsigned int died;					//number of times player has died
		unsigned int attacked;				//total player has attacked successfully
		unsigned int dodged;				//total number has dodged successfully

		room(int limit_ = 5);
	};

	std::list<room> rooms;					//list storing room

	int performance;						//current player performance score
	unsigned int next_limit;				//maximum number of enemies to be spaned in next room

	int room_number;						//number of rooms that have been cleared
	unsigned int max_rooms;					//maximum number of rooms stored in rooms

	void next(int limit = 5);				//adds a room and ends combat in previous room if it exists
	void evaluate();						//evaluates player performance

protected:
	room_handler();

	//logistics
	bool can_spawn();						//returns whether more enemies can be spawned
	void room_over();						//to be called when room is complete
	void reset();							//resets room's spawns
	int get_performance();					//returns performance score

	//metrics callback functions
	void turnOver();						//notifies room that turn is over
	void playerDodge();						//notifies room of player dodging enemy attack
	void playerAttack();					//notifies room of player missing attack
	void enemySpawn();						//notifies room of enemy spawn
	void enemyDie();						//notifies room of enemy death
	void playerDie();						//notifies room of player death
};

#endif
