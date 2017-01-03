#include <vector>

using namespace std;

class Corner;
class Edge;
class Polygon;

class XY
{
	public:
		int x;
		int y;
};

class Corner
{
	public:
		vector<Edge> edges;
		vector<Polygon> polygons;
		XY corner;
};

class Edge
{
	public:
		vector<Corner> corners;
		vector<Polygon> polygons;
};

class Polygon
{

	public:	
		vector<Corner> corners;
		vector<Edge> edges;
		XY center;
};