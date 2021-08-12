/*
* Justin W Li
* Enemy handler function implementations
*/

#include "enemy_handler.h"
#include <algorithm> //for_each
#include <cmath>

//enemy ctor implementation
enemy_handler::enemy::enemy(int type_) : type(type_) {
	//just init values based on inputted enemy type
	switch (type)
	{
	case GOBLIN:
		name = "GOBLIN";
		hp = 10;
		atk = 5;
		exp = 3;
		break;
	case GOB_SHAMAN:
		name = "GOB_SHAMAN";
		hp = 20;
		atk = 10;
		exp = 9;
		break;
	case HOBGOBLIN:
		name = "HOBGOBLIN";
		hp = 50;
		atk = 15;
		exp = 27;
		break;
	case GOB_LORD:
		name = "GOB_LORD";
		hp = 100;
		atk = 45;
		exp = 81;
		break;
	case GOB_PALADIN:
		name = "GOB_PALADIN";
		hp = 200;
		atk = 100;
		exp = 243;
		break;
	default:
		throw "Invalid enemy type inputted!";
		break;
	}
}

//enemy handler ctor
enemy_handler::enemy_handler() : enemies(), stage(0), threshholds() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));		//create seed
	set_thresholds();												//properly init threshholds
}

//sets stage value based on performance
void enemy_handler::set_stage(int performance_) {	
	//set stage based on performance value
	if (performance_ < 5)
		stage = 0;
	else if (performance_ < 10)
		stage = 1;
	else if (performance_ < 20)
		stage = 2;
	else if (performance_ < 30)
		stage = 3;
	else if (performance_ < 40)
		stage = 4;
	else if (performance_ < 60)
		stage = 5;
	else if (performance_ < 80)
		stage = 6;
	else if (performance_ < 100)
		stage = 7;
	else if (performance_ < 120)
		stage = 8;
	else if (performance_ < 140)
		stage = 9;
	else if (performance_ < 160)
		stage = 10;
	else if (performance_ < 180)
		stage = 11;
	else if (performance_ < 210)
		stage = 12;
	else if (performance_ < 240)
		stage = 13;
	else
		stage = 14;
}

//calculates threshholds
void enemy_handler::set_thresholds() {
	//sets threshholds based on range of values that performance is in
	switch (stage)
	{
	case 0:
		threshholds[GOBLIN] = 100;		//100%
		threshholds[GOB_SHAMAN] = 0;	//0%
		threshholds[HOBGOBLIN] = 0;		//0%
		threshholds[GOB_LORD] = 0;		//0%
		threshholds[GOB_PALADIN] = 0;	//0%
		break;	
	case 1:
		threshholds[GOBLIN] = 80;		//80%
		threshholds[GOB_SHAMAN] = 100;	//20%
		threshholds[HOBGOBLIN] = 0;		//0%
		threshholds[GOB_LORD] = 0;		//0%
		threshholds[GOB_PALADIN] = 0;	//0%
		break;
	case 2:
		threshholds[GOBLIN] = 60;		//60%
		threshholds[GOB_SHAMAN] = 90;	//30%
		threshholds[HOBGOBLIN] = 100;	//10%
		threshholds[GOB_LORD] = 0;		//0%
		threshholds[GOB_PALADIN] = 0;	//0%
		break;
	case 3:
		threshholds[GOBLIN] = 40;		//40%
		threshholds[GOB_SHAMAN] = 70;	//30%
		threshholds[HOBGOBLIN] = 90;	//20%
		threshholds[GOB_LORD] = 100;	//10%
		threshholds[GOB_PALADIN] = 0;	//0%
		break;
	case 4:
		threshholds[GOBLIN] = 30;		//30%
		threshholds[GOB_SHAMAN] = 60;	//30%
		threshholds[HOBGOBLIN] = 80;	//20%
		threshholds[GOB_LORD] = 90;		//10%
		threshholds[GOB_PALADIN] = 100;	//10%
		break;
	case 5:
		threshholds[GOBLIN] = 20;		//20%
		threshholds[GOB_SHAMAN] = 45;	//25%
		threshholds[HOBGOBLIN] = 65;	//20%
		threshholds[GOB_LORD] = 85;		//20%
		threshholds[GOB_PALADIN] = 100;	//15%
		break;
	case 6:
		threshholds[GOBLIN] = 10;		//10%
		threshholds[GOB_SHAMAN] = 30;	//20%
		threshholds[HOBGOBLIN] = 60;	//30%
		threshholds[GOB_LORD] = 85;		//25%
		threshholds[GOB_PALADIN] = 100;	//15%
		break;
	case 7:
		threshholds[GOBLIN] = 0;		//0%
		threshholds[GOB_SHAMAN] = 10;	//10%
		threshholds[HOBGOBLIN] = 45;	//35%
		threshholds[GOB_LORD] = 80;		//35%
		threshholds[GOB_PALADIN] = 100;	//20%
		break;
	case 8:
		threshholds[GOBLIN] = 0;		//0%
		threshholds[GOB_SHAMAN] = 0;	//0%
		threshholds[HOBGOBLIN] = 30;	//30%
		threshholds[GOB_LORD] = 70;		//40%
		threshholds[GOB_PALADIN] = 100;	//30%
		break;
	case 9:
		threshholds[GOBLIN] = 0;		//0%
		threshholds[GOB_SHAMAN] = 0;	//0%
		threshholds[HOBGOBLIN] = 20;	//20%
		threshholds[GOB_LORD] = 60;		//40%
		threshholds[GOB_PALADIN] = 100;	//40%
		break;
	case 10:
		threshholds[GOBLIN] = 0;		//0%
		threshholds[GOB_SHAMAN] = 0;	//0%
		threshholds[HOBGOBLIN] = 5;		//5%
		threshholds[GOB_LORD] = 55;		//55%
		threshholds[GOB_PALADIN] = 100;	//45%
		break;
	case 11:
		threshholds[GOBLIN] = 0;		//0%
		threshholds[GOB_SHAMAN] = 0;	//0%
		threshholds[HOBGOBLIN] = 0;		//0%
		threshholds[GOB_LORD] = 45;		//45%
		threshholds[GOB_PALADIN] = 100;	//55%
		break;
	case 12:
		threshholds[GOBLIN] = 0;		//0%
		threshholds[GOB_SHAMAN] = 0;	//0%
		threshholds[HOBGOBLIN] = 0;		//0%
		threshholds[GOB_LORD] = 25;		//25%
		threshholds[GOB_PALADIN] = 100;	//75%
		break;
	case 13:
		threshholds[GOBLIN] = 0;		//0%
		threshholds[GOB_SHAMAN] = 0;	//0%
		threshholds[HOBGOBLIN] = 0;		//0%
		threshholds[GOB_LORD] = 10;		//10%
		threshholds[GOB_PALADIN] = 100;	//90%
		break;
	case 14:
		threshholds[GOBLIN] = 0;		//0%
		threshholds[GOB_SHAMAN] = 0;	//0%
		threshholds[HOBGOBLIN] = 0;		//0%
		threshholds[GOB_LORD] = 0;		//0%
		threshholds[GOB_PALADIN] = 100;	//100%
		break;
	default:
		throw "set_thresholds(): invalid stage!\n";
	}
}

void enemy_handler::spawn() {
	int enemy_type = rand() % 101;	//generate a number between 0 and 100
	for (int i = 0; i < 5; ++i) {	
		if (enemy_type < threshholds[i]) //find where random number falls in threshholds
		{
			//correct enemy type found; spawn and return
			enemies.push_back(enemy(i));
			return;
		}
	}
}

int enemy_handler::hp() { return enemies.front().hp; }
int enemy_handler::attack() { return enemies.front().atk; }
void enemy_handler::defend(int dmg) { enemies.front().hp -= dmg; }
unsigned int enemy_handler::exp() { return enemies.front().exp; }
bool enemy_handler::alive() { return enemies.front().hp > 0; }
void enemy_handler::die() { enemies.pop_front(); }
bool enemy_handler::empty() { return enemies.empty(); }
int enemy_handler::get_type() { return enemies.front().type; }
unsigned int enemy_handler::kill_all() {
	unsigned int total_exp = 0;
	while (!enemies.empty())
	{
		total_exp += exp();
		die();
	}
	return total_exp;
}
void enemy_handler::print_enemies()
{
	enemy_handler::print p;
	std::for_each(enemies.begin(), enemies.end(), p);
}
std::string enemy_handler::curr_name() { return enemies.front().name; }
std::string enemy_handler::last_name() { return enemies.back().name; }
int enemy_handler::enemies_left() { return enemies.size(); }
