/*
* Justin W Li
* room_handler.cpp
* room handler function implementations
*/

#include "room_handler.h"

//room struct ctor
room_handler::room::room(unsigned int limit_) : turns(0), limit(limit_), spawned(0), killed(0), died(0), attacked(0), dodged(0) {}

//room handler ctor
room_handler::room_handler() : rooms(), performance(0), next_limit(0), room_number(0), max_rooms(3) {
	next(); //make one room to start off
}

//proceed to next room
void room_handler::next(unsigned int limit) {
	//do not have more than the maximum number of rooms
	if (rooms.size() == max_rooms) rooms.pop_back();

	//add new, blank room to front of list
	rooms.push_front(room(limit));
	++room_number;
}

//calculate performance based on data from all rooms
void room_handler::evaluate() {
	//calculate metrics
	double percent_killed = rooms.front().killed / rooms.front().limit; //should reflect most recent room only
	
	//should reflect all previous rooms in rooms
	double percent_hit = 0.0001;
	double percent_dodged = 0;
	double times_died = 0;
	double total_turns = 0;
	std::list<room>::iterator it = rooms.begin();
	while (it != rooms.end())
	{
		percent_hit += it->attacked;
		percent_dodged += it->dodged;
		total_turns += it->turns;
		times_died += it->died;
		++it;
	}
	percent_hit /= total_turns;
	percent_dodged /= total_turns;

	//calculate, save performance score
	performance = static_cast<int>(next_limit) + room_number * static_cast<int>((percent_hit + percent_dodged + (1 - percent_killed)) * 10 / (times_died + 1));
	next_limit = static_cast<unsigned int>(room_number * 2);
}

//returns whether rooms has exceeded spawn limit
bool room_handler::can_spawn() const { return rooms.front().limit > rooms.front().spawned; } 
//updates performance metrics and adds another room
void room_handler::room_over() {
	evaluate();
	next(next_limit);
}
//resets room's spawn counter
void room_handler::reset() { rooms.front().spawned = 0; }
//returns performance score
int room_handler::get_performance() { return performance; }

//increment corresponding metric when called
void room_handler::turnOver() { ++rooms.front().turns; }		
void room_handler::playerDodge() { ++rooms.front().dodged; }	
void room_handler::playerAttack() { ++rooms.front().attacked; }	
void room_handler::playerDie() { ++rooms.front().died; }		
void room_handler::enemySpawn() { ++rooms.front().spawned; }	
void room_handler::enemyDie() { ++rooms.front().killed; }		