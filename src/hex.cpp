
#include <iostream>  
#include "hex.hpp"

using namespace std;

Hex::Hex(int q, int r)
{
	Hex::q = q;
	Hex::r = r;

	Hex::height = 0;
	Hex::movement_penalty = 0;
	Hex::defense = 0;

	Hex::_0_river = false;
	Hex::_1_river = false;
	Hex::_2_river = false;
	Hex::_3_river = false;
	Hex::_4_river = false;
	Hex::_5_river = false;

	Hex::mountain = false;
	Hex::hill = 	false;
	Hex::marsh =	false;
	Hex::forest = 	false;
	Hex::desert =	false;
	Hex::snow 	= 	false;
}


//We can use height as the metric to determine line of sight and how far a unit can see.
//We can also use height to determine whether something is classified as a mountain, hill or plains. 
//This way height should also influence if something is a mountain, hills or plains
void Hex::set_Height(int height) 
{
	Hex::height = height;
	if(height > MIN_MOUNTAIN_HEIGHT) {
		Hex::mountain = true; 
		Hex::movement_penalty += MOUNTAIN_MOVEMENT_PENALTY; 
		Hex::defense += MOUNTAIN_DEFENSE;
	}
	else if(height > MIN_HILL_HEIGHT) {
		Hex::hill = true; 
		Hex::movement_penalty += HILL_MOVEMENT_PENALTY; 
		Hex::defense += HILL_DEFENSE;}
}

//TODO:
//Properly give all values
void Hex::add_Forests() 
{
	if(!Hex::marsh && !Hex::desert && !Hex::mountain)
	{
		Hex::forest = true; 
		Hex::movement_penalty += FOREST_MOVEMENT_PENALTY; 
		Hex::defense += FOREST_DEFENSE;
	}
}

void Hex::add_Marsh() 		
{
	if(!Hex::mountain && !Hex::hill && !Hex::forest && !Hex::desert && !Hex::snow) 
	{
		Hex::marsh = true;
		Hex::movement_penalty += MARSH_MOVEMENT_PENALTY; 
		Hex::defense += MARSH_DEFENSE;
	}
}

void Hex::add_Desert() 		
{
	if(!Hex::marsh && !Hex::forest && !Hex::snow) 
	{
		Hex::desert = true; 
		Hex::movement_penalty += DESERT_MOVEMENT_PENALTY; 
		Hex::defense += DESERT_DEFENSE;
	}
}

void Hex::add_Snow()		
{
	if(!Hex::marsh && !Hex::desert)
	{
		Hex::snow = true; 
		Hex::movement_penalty += SNOW_MOVEMENT_PENALTY; 
		Hex::defense += SNOW_DEFENSE;
	}
}

void Hex::add_River(int id)
{

}

void Hex::add_Unit(Unit *unit) 
{
	Hex::unit = unit;
}

void Hex::remove_Unit() 
{
	Hex::unit = NULL;
}