/*
* Justin W Li
* game_loop.h
* game loop class definition
*/

#include "event_handler.h"
#include "enemy_handler.h"
#include "room_handler.h"
#include "word_handler.h"
#include "player.h"

//runs the game loop
//multiply inherits from all other handlers
class game_loop : public event_handler, public enemy_handler,
	public room_handler, public word_handler, public player 
{
	event_handler evh;
	enemy_handler enh;
	room_handler rh;
	word_handler wh;
	player p;

public:
	//ctor
	game_loop();
	void run();
};