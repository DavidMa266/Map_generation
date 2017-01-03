#include "map.hpp"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

int q_direction(int q, int dir) {return q + q_directions[dir%6];}
int r_direction(int r, int dir) {return r + r_directions[dir%6];}



Hex get_hex(Hex origin, int dir)
{
	return Hex(0,0);
}

