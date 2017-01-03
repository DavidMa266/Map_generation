#include "unit.hpp"

class Hex

{
	private:
	    int q;
	    int r;
	    int height;
		int movement_penalty;
		int defense;

		bool _0_river;
		bool _1_river;
		bool _2_river;
		bool _3_river;
		bool _4_river;
		bool _5_river;

		bool mountain;
		bool hill;
		bool marsh;
		bool desert;
		bool forest;
		bool snow;
		Unit *unit;
	public:
	    Hex(int q, int r);
	    void set_Height(int height);

	    void add_Forests();
	    void add_Marsh();
	    void add_Desert();
	    void add_Snow();
	    void add_River(int id);
	    void add_Unit(Unit *unit);
	    void remove_Unit();

};

/**_____________________________________________
|												|
|			Elevation and Rivers				|
|_______________________________________________|
|												|
|		We have two different methods for		|
|		River generation.						|
|												|
|		Method 1 is Civ 5:						|
|			Rivers live in between hexes		|
|												|
|		Method 2 is Fire Emblem:				|
|			Rivers become their own tile		|
|												|
|		The method chosen is likely dependent	|
|		on the type of elevation map			|
|												|
|		A more detatched 'easy' map is more 	|
|		suited to Method 1 						|
|		More difficult specific maps are 2.		|
|_______________________________________________|*/


const int MIN_MOUNTAIN_HEIGHT = 1000;
const int MOUNTAIN_MOVEMENT_PENALTY = 4;
const int MOUNTAIN_DEFENSE = 10;

const int MIN_HILL_HEIGHT = 500;
const int HILL_MOVEMENT_PENALTY = 2;
const int HILL_DEFENSE = 5;

const int FOREST_MOVEMENT_PENALTY = 2;
const int FOREST_DEFENSE = 5;

const int MARSH_MOVEMENT_PENALTY = 2;
const int MARSH_DEFENSE = -5;

const int DESERT_MOVEMENT_PENALTY = 1;
const int DESERT_DEFENSE = 0;

const int SNOW_MOVEMENT_PENALTY = 0;
const int SNOW_DEFENSE = 0;

