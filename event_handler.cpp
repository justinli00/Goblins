/*
* Justin W Li
* event_handler.cpp
* Event handler function implementations
* Event function implementations
*/

#include "event_handler.h"

//-------------------------------------
//----EVENT HANDLER IMPLEMENTATIONS----
//-------------------------------------

//ctor
event_handler::event_handler() : events(), curr_event(nullptr) {}

//dtor
event_handler::~event_handler() { clear_events(); } //delete each event in the event list

//comparison function
bool event_handler::comp::operator()(game_event* lhs, game_event* rhs) const
{
	return lhs->get_prio() < rhs->get_prio();
}

//adds event to queue
void event_handler::add_event(game_event* e) {
	if (e == nullptr) 
	{
		throw event_handler::bad_event();
		return;
	}

	//make pair and push
	events.push(e);
}

//replaces current event with event at top of queue; pops queue
//returns true if operation was performed successfully, otherwise returns false
bool event_handler::next_event() {
	if (curr_event != nullptr)
	{
		delete curr_event;
		curr_event = nullptr;
	}
	if (!events.empty())
	{
		curr_event = events.top();
		events.pop();
		return true;
	}
	else return false;
}

void event_handler::run_events() {
	//execute all events
	while (!events.empty())
		events.top()->start_event();
}

void event_handler::clear_events() {
	//delete current event
	if (curr_event != nullptr)
	{
		delete curr_event;
		curr_event = nullptr;
	}

	//delete every event in queue
	while (!events.empty())
	{
		game_event* e = events.top();
		events.pop();
		delete e;
	}
}

//returns type of event at top of queue; returns -1 if queue is empty
int event_handler::top_prio() const {
	if (events.empty()) return -1;
	else return events.top()->get_prio();
}

//----------------------------------
//----BASE EVENT IMPLEMENTATIONS----
//----------------------------------

//event function implementations
game_event::game_event(event_handler* evh_, room_handler* rh_, int prio, 
	void (room_handler::* pNotify_)()) :
	evh(evh_), rh(rh_), priority(prio), pNotify(pNotify_) {
	if (evh_ == nullptr) throw EVENT_EXCEPTION("Invalid event handler pointer!\n");
	if (rh_ == nullptr) throw EVENT_EXCEPTION("Invalid room handler pointer!\n");
}

void game_event::start_event() { 
	//delete current event if necessary, then place event at top of queue into current_event
	if(evh->next_event()) run_event();
}

void game_event::complete_event() { 
	if (priority == INPUT)
		std::cout << std::endl;	//print newline for readability

	if((priority == COMBAT || priority == ROOM_OVER) && evh->top_prio() != FEEDBACK)
	{
		if(priority == COMBAT)
			std::cout << "Press enter to start combat." << std::endl;		//let player know that they're entering combat
		else
			std::cout << "Press enter to continue." << std::endl;			//prompt player to press enter
		while (std::cin.get() != '\n');
	}

	//notify room_handler of event, if notification function was inputted
	if(pNotify != nullptr) (rh->*pNotify)();
}
int game_event::get_prio() const { return priority; }

enemy_event::enemy_event(event_handler* evh_, room_handler* rh_, 
	enemy_handler* enh_, int prio,
	void (room_handler::* pNotify_)(), std::string(enemy_handler::* pName_)() const) try :
	game_event(evh_, rh_, prio, pNotify_), enh(enh_), pName(pName_) {
	if (enh_ == nullptr) throw EVENT_EXCEPTION("Invalid enemy handler pointer!\n");
	if (pName_ == nullptr) throw EVENT_EXCEPTION("Invalid enemy name function pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

player_event::player_event(event_handler* evh_, room_handler* rh_, player* p_, int prio, 
	void (room_handler::* pNotify_)()) try :
	game_event(evh_, rh_, prio, pNotify_), p(p_) {
	if (p_ == nullptr) throw EVENT_EXCEPTION("Invalid player pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

//input reception event
input_event::input_event(event_handler* evh_, room_handler* rh_, word_handler* wh_,
	game_event* pPassEvent_, game_event* pFailEvent_, int prio) try :
	game_event(evh_, rh_, prio), wh(wh_), pPassEvent(pPassEvent_), pFailEvent(pFailEvent_) { 
	if (wh_ == nullptr) throw EVENT_EXCEPTION("Invalid word handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

//------------------------------------
//----GAMESTATE/ROOM EVENT CLASSES----
//------------------------------------

//load event
game_load::game_load(event_handler* evh_, room_handler* rh_, word_handler* wh_,	int prio) try :
	game_event(evh_, rh_, prio), wh(wh_) {
	if (wh_ == nullptr) throw EVENT_EXCEPTION("Invalid word handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void game_load::run_event() {
	std::cout << "Loading word bank..." << std::endl;
	wh->load_bank();
	complete_event();
}

//intro event
game_intro::game_intro(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
	player* p_, int prio) try :
	game_event(evh_, rh_, prio), wh(wh_), p(p_) {
	if (wh_ == nullptr) throw EVENT_EXCEPTION("Invalid word handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}


void game_intro::run_event() {
	//print intro
	std::cout << "Welcome to Goblins! Prepare to go from room to room in a dungeon." << std::endl;
	std::cout << "In each room, goblins will line up to fight you, one at a time." << std::endl;
	std::cout << "Attack the goblins and dodge their attacks by quickly typing in the words they throw at you!" << std::endl;
	std::cout << "Be warned that if you either spell the word wrong or fail to type it in time," << std::endl;
	std::cout << "you will either miss your attack or get hit by that of goblin's. It's not case-sensitive, though." << std::endl;
	std::cout << "You'll move to the next room once there are no goblins left in line." << std::endl;

	try {
		//add start/exit event
		evh->add_event(new non_combat_event(evh, rh, wh,
			new player_start(evh, rh, p),
			new player_quit(evh, rh, p),
			"Start", "Quit"));
	}
	catch (std::bad_alloc& e) { //check for alloc failure
		throw e.what(); 
	}

	complete_event();
}

//turn over event for room
turn_over::turn_over(event_handler* evh_, room_handler* rh_, int prio,
	void (room_handler::* pNotify_)()) try :
	game_event(evh_, rh_, prio, pNotify_) {}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void turn_over::run_event() {
	complete_event(); //just complete the event
}

//room over event for room
room_over::room_over(event_handler* evh_, room_handler* rh_, enemy_handler* enh_, 
	int prio, void (room_handler::* pNotify_)()) try :
	game_event(evh_, rh_, prio, pNotify_), enh(enh_) {
	if (enh_ == nullptr) throw EVENT_EXCEPTION("Invalid enemy handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void room_over::run_event() {
	std::cout << "There are no more goblins in the room. You go and step into the next room." << std::endl;
	rh->room_over();						//evaluate player performance
	enh->set_stage(rh->get_performance());	//update stage
	enh->set_thresholds();					//update thresholds
	complete_event();
}

//-----------------------------------------
//----INPUT EVENT CLASS IMPLEMENTATIONS----
//-----------------------------------------

combat_event::combat_event(event_handler* evh_, room_handler* rh_,
	word_handler* wh_, enemy_handler* enh_, game_event* pPassEvent_,
	game_event* pFailEvent_, int prio) try :
	input_event(evh_, rh_, wh_, pPassEvent_, pFailEvent_, prio),enh(enh_) { 
	if (enh_ == nullptr) throw EVENT_EXCEPTION("Invalid enemy handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void combat_event::run_event() {
	//time that event started checking for input
	std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();	

	//generate a string based on the type
	std::string str = wh->get_string(enh->get_type());

	//prompt user for string
	std::string user_str;
	std::cout << "Type \"" << str << "\"." << std::endl;
	std::cin >> user_str;

	//calculate time to complete
	unsigned int wait_t = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now() - start_t).count());

	//check that strings match, and that maximum time wasn't exceeded
	if (wh->string_compare(str, user_str) && wait_t < (str.size() * 250 + 1500)) //250 milliseconds per letter, plus 1.5 seconds to read 
	{
		try {
			evh->add_event(pPassEvent);		//event failed, add fail event 
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}
		delete pFailEvent;					//fail event not needed; delete
	}
	else 
	{
		try {
			evh->add_event(pFailEvent);		//event failed, add fail event 
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}
		delete pPassEvent;					//pass event not needed; delete
	}

	//wait for user to lift enter
	while (std::cin.get() != '\n');
	complete_event();
}

non_combat_event::non_combat_event(event_handler* evh_, room_handler* rh_, 
	word_handler* wh_, game_event* pPassEvent_, game_event* pFailEvent_, 
	std::string pass_, std::string fail_, int prio) try :
	input_event(evh_, rh_, wh_, pPassEvent_, pFailEvent_, prio),
	pass(pass_), fail(fail_) 
{
	//check that pass, fail don't match
	if (wh->string_compare(pass, fail))
	{
		//destroy pass/fail events, throw exception
		delete pPassEvent;
		delete pFailEvent;
		throw game_event::EVENT_EXCEPTION("Pass, fail strings cannot match!\n");
	}
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void non_combat_event::run_event() {

	std::string user_str;
	bool pass_event = false;
	bool fail_event = false;
	do {
		//prompt user for string until entry matches either pass string or fail string
		std::cout << "Type \"" << pass << "\" or \"" << fail << "\"." << std::endl;
		std::cin >> user_str;
		pass_event = wh->string_compare(user_str, pass); 
		fail_event = wh->string_compare(user_str, fail);

		if (!(pass_event || fail_event))
		{
			//input matches neither; prompt user to try again
			std::cout << "Sorry, input was not recognized. Please try again." << std::endl;
		}

	} while (!pass_event && !fail_event);

	if (pass_event)
	{
		try {
			evh->add_event(pPassEvent);	//event failed, add fail event 
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}
		delete pFailEvent;			//pass event not needed; delete
	}
	else
	{
		try {
			evh->add_event(pFailEvent);		//event failed, add fail event 
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}
		delete pPassEvent;				//pass event not needed; delete
	}

	while (std::cin.get() != '\n');
	complete_event();
}

//-----------------------------------------
//----ENEMY EVENT CLASS IMPLEMENTATIONS----
//-----------------------------------------

//enemy spawn event function implementations
enemy_spawn::enemy_spawn(event_handler* evh_, room_handler* rh_, enemy_handler* enh_, 
	int prio, void (room_handler::* pNotify_)()) try :
	enemy_event(evh_, rh_, enh_, prio, pNotify_, &enemy_handler::last_name) {}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void enemy_spawn::run_event() {
	enh->spawn();
	std::cout << "A " << (enh->*pName)() << " just got in line!" << std::endl;
	std::cout << "HP: " << enh->hp_back() << ", " << "ATK: " << enh->attack_back() << std::endl;

	int gobs_left = enh->enemies_left();
	if (gobs_left != 1)
		std::cout << "There are now " << gobs_left << " goblins in line." << std::endl;
	else
		std::cout << "There is now " << gobs_left << " goblin in line." << std::endl; //singular
	complete_event();
}

//enemy attack event function implementations
enemy_attack::enemy_attack(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
	enemy_handler* enh_, player* p_, int prio) try :
	enemy_event(evh_, rh_, enh_, prio), p(p_), wh(wh_) {
	if (p_ == nullptr) throw EVENT_EXCEPTION("Invalid player pointer!\n");
	if (wh_ == nullptr) throw EVENT_EXCEPTION("Invalid word handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void enemy_attack::run_event() {
	//check that there are enemies
	if (!enh->empty())
	{
		std::cout << "The " << (enh->*pName)() << " is attacking!" << std::endl;

		try {
			//create input event
			evh->add_event(static_cast<game_event*>(new combat_event(evh, rh, wh, enh,
				static_cast<game_event*>(new player_dodge(evh, rh, p)),					 //event success
				static_cast<game_event*>(new player_defend(evh, rh, wh, enh, p, enh->attack())) //event fail
			)));
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}
		complete_event();
	}
}
//enemy defense event function implementations
enemy_defend::enemy_defend(event_handler* evh_, room_handler* rh_, enemy_handler* enh_, 
	player* p_, int prio, void (room_handler::* pNotify_)()) try :
	enemy_event(evh_, rh_, enh_, prio, pNotify_), p(p_) {
	if (p_ == nullptr) throw EVENT_EXCEPTION("Invalid player pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void enemy_defend::run_event() {
	std::cout << "The " << (enh->*pName)() << " takes " << p->attack() << " damage!" << std::endl;
	enh->defend(p->attack());

	//check if enemy is alive; kill if dead
	if (!enh->alive())
	{
		try {
			//add enemy death event
			evh->add_event(static_cast<game_event*>(new enemy_die(evh, rh, enh, p)));
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}
	}
	else
		std::cout << "It still has " << enh->hp() << " hp." << std::endl;
	complete_event();
}

//enemy death event function implementations
enemy_die::enemy_die(event_handler* evh_, room_handler* rh_, enemy_handler* enh_, 
	player* p_, int prio, void (room_handler::* pNotify_)()) try :
	enemy_event(evh_, rh_, enh_, prio, pNotify_), p(p_) {
	if (p_ == nullptr) throw EVENT_EXCEPTION("Invalid player pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void enemy_die::run_event() {
	std::cout << "The " << (enh->*pName)() << " is dead!" << std::endl;
	//give player exp
	try {
		//add player exp event
		evh->add_event(static_cast<game_event*>(new player_exp(evh, rh, p, enh->exp())));
	}
	catch (std::bad_alloc& e) { //check for alloc failure
		throw e.what();
	}

	//kill enemy
	enh->die();

	int gobs_left = enh->enemies_left();
	if (gobs_left)
	{
		//enemy at front of new line is different; notify player
		std::cout << "A " << (enh->*pName)() << " steps up to take its place." << std::endl;
		if (gobs_left != 1)
			std::cout << "There are " << gobs_left << " goblins left in line." << std::endl;
		else
			std::cout << "There is " << gobs_left << " goblin left in line." << std::endl; //singular
	}
	else
	{
		//create a new room
		try {
			evh->add_event(static_cast<game_event*>(new room_over(evh, rh, enh)));
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}
	}
	complete_event();
}

//------------------------------------------
//----PLAYER EVENT CLASS IMPLEMENTATIONS----
//------------------------------------------

player_attack::player_attack(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
	enemy_handler* enh_, player* p_, int prio) try :
	player_event(evh_, rh_,p_, prio), enh(enh_), wh(wh_), pName(&enemy_handler::curr_name) {
	if (enh_ == nullptr) throw EVENT_EXCEPTION("Invalid enemy handler pointer!\n");
	if (wh_ == nullptr) throw EVENT_EXCEPTION("Invalid word handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}


void player_attack::run_event(){
	std::cout << "Attack the " << (enh->*pName)() << "!" << std::endl;
	//create input event
	try {
		//add player exp event
		evh->add_event(static_cast<game_event*>(new combat_event(evh, rh, wh, enh,
			new enemy_defend(evh, rh, enh, p),
			new player_miss(evh, rh, p))));
	}
	catch (std::bad_alloc& e) { //check for alloc failure
		throw e.what();
	}
	complete_event();
}

//player dodge event
player_miss::player_miss(event_handler* evh_, room_handler* rh_, player* p_, int prio) try :
	player_event(evh_, rh_, p_, prio) {}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}
void player_miss::run_event() {
	std::cout << "You missed!" << std::endl;
	complete_event();
}

//player dodge event function implementations
player_dodge::player_dodge(event_handler* evh_, room_handler* rh_, player* p_, int prio,
	void (room_handler::* pNotify_)()) try :
	player_event(evh_, rh_, p_, prio, pNotify_) {}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}
void player_dodge::run_event() {
	std::cout << "You dodged the attack!" << std::endl;
	complete_event();
}

//player defense function implementations
player_defend::player_defend(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
	enemy_handler* enh_, player* p_,	int dmg_, int prio) try :
	player_event(evh_, rh_, p_, prio), wh(wh_), enh(enh_), dmg(dmg_) {
	if (enh_ == nullptr) throw EVENT_EXCEPTION("Invalid enemy handler pointer!\n");
	if (wh_ == nullptr) throw EVENT_EXCEPTION("Invalid word handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void player_defend::run_event() {
	std::cout << "You got hit and took " << dmg << " damage." << std::endl;
	p->defend(dmg);
	if (!p->alive()) //kill player if hp drops below zero
	{
		try {
			//add player exp event
			evh->add_event(static_cast<game_event*>(new player_die(evh, rh, wh, enh, p)));
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}
	}
	else
		std::cout << "You still have " << p->health() << " hp." << std::endl;
	complete_event();
}

//player exp gain function implementations
player_exp::player_exp(event_handler* evh_, room_handler* rh_, player* p_, 
	int exp_, int prio) try :
	player_event(evh_, rh_, p_, prio), exp(exp_) {}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void player_exp::run_event() {
	std::cout << "You gained " << exp << " EXP!" << std::endl;
	p->gain_exp(exp);

	//check that player has leveled up
	if (p->can_level())
	{
		try {
			//add player exp event
			evh->add_event(static_cast<game_event*>(new player_levelup(evh, rh, p)));
		}
		catch (std::bad_alloc& e) { //check for alloc failure
			throw e.what();
		}
	}
	complete_event();
}

//player level up event function implementations
player_levelup::player_levelup(event_handler* evh_, room_handler* rh_, 
	player* p_, int prio) try :
	player_event(evh_, rh_, p_, prio) { }
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void player_levelup::run_event() {
	std::cout << "You leveled up!" << std::endl;
	p->level_up();
	p->print_stats();
	complete_event();
}

//player death function implementations
player_die::player_die(event_handler* evh_, room_handler* rh_, word_handler* wh_, 
	enemy_handler* enh_, player* p_, int prio, void (room_handler::* pNotify_)()) try :
	player_event(evh_, rh_, p_, prio, pNotify_), wh(wh_), enh(enh_) {
	if (enh_ == nullptr) throw EVENT_EXCEPTION("Invalid enemy handler pointer!\n");
	if (wh_ == nullptr) throw EVENT_EXCEPTION("Invalid word handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void player_die::run_event() {
	std::cout << "You died. Would you like to continue?" << std::endl;
	try {
		//add choice to continue/quit
		evh->add_event(new non_combat_event(evh, rh, wh,
			new player_continue(evh, rh, enh, p),
			new player_quit(evh, rh, p)));
	}
	catch (std::bad_alloc& e) { //check for alloc failure
		throw e.what();
	}
	complete_event();
}

player_continue::player_continue(event_handler* evh_, room_handler* rh_, enemy_handler* enh_,
	player* p_, int prio) try :
	player_event(evh_, rh_, p_, prio), enh(enh_) {
	if (enh_ == nullptr) throw EVENT_EXCEPTION("Invalid enemy handler pointer!\n");
}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void player_continue::run_event() {
	//restart the room, but keep most metrics
	std::cout << "Reseting the room." << std::endl;
	p->fully_heal();
	rh->reset();
	enh->kill_all();
	complete_event();
}

player_start::player_start(event_handler* evh_, room_handler* rh_, player* p_, int prio) try :
	player_event(evh_, rh_, p_, prio) {}
catch (const game_event::EVENT_EXCEPTION& e) { //catch exceptions from initializer list
	throw e.what();
}

void player_start::run_event() {
	std::cout << "You walk into the dungeon. It smells like goblin in here!" << std::endl;
	complete_event();
}

player_quit::player_quit(event_handler* evh_, room_handler* rh_, player* p_, int prio) :
	player_event(evh_, rh_, p_, prio) {}
void player_quit::run_event() {
	std::cout << "Thanks for playing!" << std::endl; //todo -- score/leaderboard stuff here
	p->game_over();			//ensure hp is zero; player is dead
	evh->clear_events();	//clear queue
	//do not call complete_event; no need to call callback or wait for user
}