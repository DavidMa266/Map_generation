#include <iostream>
#include <vector>
#include "hex.hpp"

using namespace std;

static const int q_directions[] = {1,0,-1,-1,0,1};
static const int r_directions[] = {0,1,1,0,-1,-1};

int q_direction(int q, int dir);
int r_direction(int r, int dir);


Hex get_hex(Hex origin, int dir);



vector<vector<double> > perlin_noise(int n);
vector<vector<int> >  map_generator(int width, int height, int num_seeds);

vector<vector<int> > fortune_line_sweep(int dim, int num);