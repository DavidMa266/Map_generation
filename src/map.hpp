#include <iostream>
#include <vector>
#include "hex.hpp"

using namespace std;

static const int q_directions[] = {1,0,-1,-1,0,1};
static const int r_directions[] = {0,1,1,0,-1,-1};

int q_direction(int q, int dir);
int r_direction(int r, int dir);


Hex get_hex(Hex origin, int dir);
