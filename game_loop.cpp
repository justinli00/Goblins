/*
* Justin W Li
* game_loop.cpp
* game loop class implementation
*/

#include "game_loop.h"

game_loop::game_loop() try : evh(), enh(), rh(), wh(), p() {} //just call default ctors
catch(game_event::EVENT_EXCEPTION& e) {
	throw e.what();
}
//gameplay loop function
void game_loop::run() {

	//add game load events
	try {
		evh.add_event(new game_load(&evh, &rh, &wh));
	}
	catch (std::bad_alloc& e) { //check for alloc failure
		throw e.what();
	}

	//add game intro events
	try {
		evh.add_event(new game_intro(&evh, &rh, &wh, &p));
	}
	catch (std::bad_alloc& e) { //check for alloc failure
		throw e.what();
	}

	while (p.alive())
	{
		//always add an event that ends the turn
		try {
			evh.add_event(new turn_over(&evh, &rh));
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}

		if (rh.can_spawn()) //check if more enemies can be spawned
		{
			//spawn an enemy
			try {
				evh.add_event(new enemy_spawn(&evh, &rh, &enh));
			}
			catch (std::bad_alloc& e) { //check for alloc failure
				throw e.what();
			}
		}

		//check if there are enemies
		if (!enh.empty()) {
			//add combat functions
			//spawn an enemy
			try {
				evh.add_event(new player_attack(&evh, &rh, &wh, &enh, &p));
				evh.add_event(new enemy_attack(&evh, &rh, &wh, &enh, &p));
			}
			catch (std::bad_alloc& e) { //check for alloc failure
				throw e.what();
			}
		}
		//else if (!rh.can_spawn()) //no enemies and no enemies to be spawned
		//{
		//	//room complete
		//	//todo -- evh.add_event(new room_over(&evh, &rh, &enh));
		//} //todo - -do you still even need this?

		//run all events in the queue
		evh.run_events();
	}
}

