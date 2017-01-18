#include <unordered_map>
#include <vector>
#include <bitset>
#include <string>
#include <utility>
#include <unordered_map>

using namespace std;

class Vertex;
class Edge;
class Polygon;
class Tree;

class XY
{
	public:
		double x;
		double y;
		bool operator==(const XY &xy) const {return (xy.x==this->x && xy.y==this->y);}
		XY(double x, double y) : x(x), y(y){}
};

namespace std {

	template <>
	struct hash<XY>
	{	
		size_t operator()(const XY& xy) const
		{            
			size_t res = 17;
			res = res * 31 + xy.x;
			res = res * 31 + xy.y;
			return res;
		}
	};		
}

class Vertex
{
	public:
		vector<Edge*> edges;
		vector<Polygon*> polygons;
		XY* vertex;
		Vertex(XY* xy) : vertex(xy), edges(), polygons() {}
		Vertex(XY xy) : vertex(new XY(xy.x, xy.y)), edges(), polygons() {}
};

class Edge
{
	public:
		XY* xy;
		Vertex* v1;
		Vertex* v2;
		Polygon* p1;
		Polygon* p2;
		Edge* opposite;
		Edge* next;
		Edge* prev;
		Edge(XY* xy, Vertex* c1, Vertex* c2, Polygon* p1, Polygon* p2) : xy(xy), v1(c1), v2(c2), p1(p1), p2(p2) {}
		Edge(XY xy, Vertex* c1, Vertex* c2, Polygon* p1, Polygon* p2) : xy(new XY(xy.x, xy.y)), v1(c1), v2(c2), p1(p1), p2(p2) {}
		void assign_opposite(Edge* opp);
		void assign_prev(Edge* prev);
		void assign_next(Edge* next);
};

class Polygon
{

	public:	
		vector<Vertex*> vertices;
		vector<Edge*> edges;
		XY* xy;
		Polygon(XY* xy) : xy(xy),  vertices(), edges() {}
		Polygon(XY xy) : xy(new XY(xy.x, xy.y)), vertices(), edges() {}
};


class Map
{
	public:
		unordered_map<XY, Vertex*> vertices;
		unordered_map<XY, Polygon*> polygons;
		unordered_map<XY, Edge*> edges;
		Map() : vertices(), polygons(), edges() {}
};

struct Node
{
	Node* left;
	Node* right;
	Polygon* l_polygon;
	Polygon* r_polygon;
	XY* xy;

	//We should be creating this edge and passing it back?
	Edge* edge;

	//Reserved for leaf nodes only
	Polygon* arc;
};

//Don't really give a shit about time complexity atm so this'll jsut be a tree.
class Tree
{
	public:
		Node* root;
		void insert(Polygon* arc);
		Tree() : root(NULL) {}
};

void recompute_breakpoints(Node* node, int y);
bool YCompare(XY xy1, XY xy2);