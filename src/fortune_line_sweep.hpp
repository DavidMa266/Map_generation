#include "polygon.hpp"


//Need to take a 2D array and turn it into a set of polygons, corners, and edges.


class Map
{
	vector<Polygon> polygons;
	vector<Edge> edges;
	vector<Corner> corners;
};

Map line_sweep(int dim);
