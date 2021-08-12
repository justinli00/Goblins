/*
* Justin W Li
* event_handler.h
* Event handler class definition
* Event class definitions
*/

#ifndef EVENT_HANDLER_H
#define	EVENT_HANDLER_H

#include "enemy_handler.h"
#include "room_handler.h"
#include "word_handler.h"
#include "player.h"

#include <string>	//std::string
#include <chrono>   //std::chrono::steady_clock, std::chrono::duration
#include <queue>	//std::priority_queue
#include <utility>	//std::pair, std::make_pair
#include <vector>	//std::vector

//forward declarations
class game_event;									

//---------------------------
//----EVENT HANDLER CLASS----
//---------------------------

class event_handler
{
	//comparison class for priority queue
	class comp {
	public:
		bool operator()(game_event* lhs, game_event* rhs) const;
	};

	std::priority_queue<game_event*, 
		std::vector<game_event*>, comp> events;			//queue of events spawning
	game_event* curr_event;								//current event to be executed

public:

	//bad event exception
	struct bad_event : std::exception {
		const char* what() const noexcept { return "Invalid event pointer!\n"; }
	};

	event_handler();
	~event_handler();
	void add_event(game_event* e);						//adds event to queue
	bool next_event();									//replaces current event with event at top of queue; pops queue
	void run_events();									//runs all events in the gameplay loop
	void clear_events();								//clears event queue
	int top_prio() const;								//returns type of event at top of queue; returns -1 if queue is empty
};
 

//--------------------------
//----BASE EVENT CLASSES----
//--------------------------

//event base class
class game_event {
protected:
	event_handler* const evh;							//pointer to event handler
	room_handler* const rh;								//pointer to room handler
	int priority;										//priority value
	void (room_handler::*pNotify)();					//pointer to notification function

	game_event(event_handler* evh_, room_handler* rh_, int prio, 
		void (room_handler::* pNotify_)() = nullptr);
	virtual void run_event() = 0;						//performs actions associated with each event
	void complete_event();								//removes event from queue

public:
	//exceptions
	struct EVENT_EXCEPTION : std::exception {
		const char* msg;
		EVENT_EXCEPTION(const char* msg_) : msg(msg_) {}
		const char* what() const noexcept { return msg; }
	};

	enum PRIORITY { ROOM_OVER, ROOM, SPAWN, COMBAT, FEEDBACK, INPUT };	//low to high priorities, named by types of events

	virtual ~game_event() {};
	void start_event();									//pops top event and puts it on curr_event
	int get_prio() const;								//just gets priority
};

//enemy event base class
class enemy_event : public game_event {
protected:
	enemy_handler* const enh;								//pointer to enemy handler
	std::string (enemy_handler::* pName)() const;			//pointer to name getter function

	enemy_event(event_handler* evh_, room_handler* rh_, enemy_handler* enh_, int prio,
		void (room_handler::* pNotify_)() = nullptr, 
		std::string(enemy_handler::* pName_)() const = &enemy_handler::curr_name);
	void run_event() = 0;
	virtual ~enemy_event() {};
};

//player event base class 
class player_event : public game_event {
protected:
	player* const p;  //pointer to player object

	player_event(event_handler* evh_, room_handler* rh_, player* p_, int prio,
		void (room_handler::* pNotify_)() = nullptr);
	void run_event() = 0;
	virtual ~player_event() {};
};

//reads input
class input_event : public game_event {
protected:
	word_handler* const wh;									//pointer to word handler
	game_event* pPassEvent;								//pointer to pass event
	game_event* pFailEvent;								//pointer to fail event

	input_event(event_handler* evh_, room_handler* rh_, word_handler* wh_,
		game_event* pPassEvent_, game_event* pFailEvent_, int prio = INPUT);				
	void run_event() = 0;
};

//------------------------------------
//----GAMESTATE/ROOM EVENT CLASSES----
//------------------------------------

//loads relevant data for game -- called once at beginning of game
class game_load : public game_event {
	word_handler* const wh;
public: 
	game_load(event_handler* evh_, room_handler* rh_, word_handler* wh_,
		int prio = INPUT);
	void run_event();
};

//prints intro and checks if player wants to start/quit
class game_intro : public game_event {
	word_handler* const wh;
	player* p;
public:
	game_intro(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
		player* p_,	int prio = INPUT);
	void run_event();
};

//notifies room_handler that turn is over
class turn_over : public game_event {
public:
	turn_over(event_handler* evh_, room_handler* rh_, int prio = ROOM,
		void (room_handler::* pNotify_)() = &room_handler::turnOver);
	void run_event();
};

//notifies room_handler that room is over
class room_over : public game_event {
	enemy_handler* enh;
public:
	room_over(event_handler* evh_, room_handler* rh_, enemy_handler* enh_, 
		int prio = ROOM_OVER, void (room_handler::* pNotify_)() = &room_handler::room_over);
	void run_event();
};

//---------------------------
//----INPUT EVENT CLASSES----
//---------------------------

class combat_event : public input_event {
	enemy_handler* const enh;
public:
	combat_event(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
		enemy_handler* enh,	game_event* pPassEvent_, game_event* pFailEvent_,
		int prio = INPUT);
	void run_event();
};

class non_combat_event : public input_event {
	std::string pass;
	std::string fail;
public: 
	non_combat_event(event_handler* evh_, room_handler* rh_, word_handler* wh_,
		game_event* pPassEvent_, game_event* pFailEvent_, 
		std::string pass_ = "Yes", std::string fail_ = "No", int prio = INPUT);
	void run_event();
};

//---------------------------
//----ENEMY EVENT CLASSES----
//---------------------------

//todo -- add event where performance is communicated to 

//enemy spawn event
class enemy_spawn : public enemy_event {
public:
	//ctor
	enemy_spawn(event_handler* evh_, room_handler* rh_, 
		enemy_handler* enh_, int prio = SPAWN, 
		void (room_handler::* pNotify_)() = &room_handler::enemySpawn);
	void run_event();
};

//enemy attack event
class enemy_attack : public enemy_event
{
	player* const p;			//pointer to player
	word_handler* const wh;	//pointer to word handler
public:
	//ctor
	enemy_attack(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
		enemy_handler* enh_, player* p_, int prio = COMBAT);
	void run_event();
};

//enemy defense event
class enemy_defend : public enemy_event
{
	player* const p;		//pointer to player
public: 
	enemy_defend(event_handler* evh_, room_handler* rh_, enemy_handler* enh_, 
		player* p_, int prio = FEEDBACK,
		void (room_handler::* pNotify_)() = &room_handler::playerAttack);
	void run_event();
};

//enemy death event
class enemy_die : public enemy_event
{
	player* const p;									//pointer to player
public:
	enemy_die(event_handler* evh_, room_handler* rh_, enemy_handler* enh_, 
		player* p_, int prio = FEEDBACK,
		void (room_handler::* pNotify_)() = &room_handler::enemyDie);
	void run_event();
};

//----------------------------
//----PLAYER EVENT CLASSES----
//----------------------------

//player attack event
class player_attack : public player_event {
	enemy_handler* const enh;
	word_handler* const wh;
	std::string(enemy_handler::* pName)() const;	//pointer to name getter function
public:
	player_attack(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
		enemy_handler* enh_, player* p_, int prio = COMBAT);
	void run_event();
};

//player dodge event
class player_miss : public player_event
{
public:
	player_miss(event_handler* evh_, room_handler* rh_, player* p_, 
		int prio = FEEDBACK);
	void run_event();
};

//player dodge event
class player_dodge : public player_event
{
public:
	player_dodge(event_handler* evh_, room_handler* rh_, player* p_, 
		int prio = FEEDBACK, void (room_handler::* pNotify_)() = &room_handler::playerDodge);
	void run_event();
};

//player defense event
class player_defend : public player_event
{
	word_handler* const wh;
	enemy_handler* const enh;
	int dmg;                          //enemy attack value
public:
	player_defend(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
		enemy_handler* enh_, player* p_, int dmg_, int prio = FEEDBACK);
	void run_event();
};

//player gain exp event
class player_exp : public player_event
{
	int exp;				//exp to be gained
public:
	player_exp(event_handler* evh_, room_handler* rh_, player* p_, 
		int exp_, int prio = FEEDBACK);
	void run_event();
};

//player level up event
class player_levelup : public player_event
{
public:
	player_levelup(event_handler* evh_, room_handler* rh_, player* p_, 
		int prio = FEEDBACK);
	void run_event();
};

//player death event
class player_die : public player_event
{
	word_handler* const wh;
	enemy_handler* const enh;
public:
	player_die(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
		enemy_handler* enh_, player* p_, int prio = FEEDBACK,
		void (room_handler::* pNotify_)() = &room_handler::playerDie);
	void run_event();
};

//player continue event
class player_continue : public player_event
{
	enemy_handler* const enh;
public:
	player_continue(event_handler* evh_, room_handler* rh_, enemy_handler* enh_,
		player* p_, int prio = FEEDBACK);
	void run_event();
};

//player start event
class player_start : public player_event
{
public:
	player_start(event_handler* evh_, room_handler* rh_, player* p_,
		int prio = INPUT);
	void run_event();
};


//player quit event
class player_quit : public player_event
{
public:
	player_quit(event_handler* evh_, room_handler* rh_,	player* p_, 
		int prio = INPUT);
	void run_event();
};

/*todo
*	-error handling
*		-make more robust
*		-throw std::exceptions, not strings
*	-const correctness
*	-style and formatting
*		-brackets
*		-comments
*	-comments
*/

#endif