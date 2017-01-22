#include <unordered_map>
#include <vector>
#include <stdio.h>
#include <queue>
#include <functional>

using namespace std;


//X and Y values should start at (0,0) and increase to (max,max)
struct VPoint
{
	double x;
	double y;
	VPoint(double x, double y) : x(x), y(y){}
	VPoint(VPoint* point) : x(point->x), y(point->y) {}
};
bool operator==(VPoint p1, VPoint p2){return (p1.x == p2.x) && (p1.y == p2.y);}



//This is the element that goes into the priority queue. For standard site events, we do not use hte radius constructor
//Radius is used to differentiate between site and circle events.
struct PQueue
{
	VPoint* point;
	double radius;

	PQueue(VPoint* point) : point(point), radius(-1) {}
	PQueue(VPoint* point, double radius) : point(point), radius(radius) {}
};

bool QueueCompare(PQueue pq1, PQueue pq2);




namespace std {
	template <>
	struct hash<VPoint>
	{	
		size_t operator()(const VPoint& xy) const
		{            
			size_t res = 17;
			res = res * 31 + xy.x;
			res = res * 31 + xy.y;
			return res;
		}
	};		
}


struct Polygon;
struct Edge;
struct Vertex;


struct Polygon
{
	VPoint* xy;
	vector<Edge*> edges;
	vector<Vertex*> vertices;
	Polygon(VPoint* vp) : xy(vp), edges(), vertices() {}
};

struct Edge
{
	Edge* opposite;
	Edge* prev;
	Edge* next;

	Vertex* v1;
	Vertex* v2;
	Polygon* p1;
	Polygon* p2;
	//P1 is the polygon this edge goes in a counterclockwise circle of.
	//P2 is the other polygon
	//In a chain of edges, all p1 should be equal.
	Edge(Polygon* p1, Polygon* p2) : p1(p1), p2(p2) {}
};

struct Vertex
{
	VPoint* xy;
	vector<Edge*> edges;
	vector<Polygon*> polygons;
	Vertex(VPoint* vp) : xy(vp), edges(), polygons() {}

};

struct Node 
{
	Node* left;
	Node* right;
	Node* parent;

	//Inner nodes are breakpoints. Its the boundary between the arcs of two polygons.
	//Also contains a pointer to a half edge.
	Polygon* l_poly;
	Polygon* r_poly;
	Edge* edge;

	//Leaf nodes have no left or right nodes. They contain Arcs
	Polygon* arc;

	//Leaf node. Only has an arc and parent. Parent must be manually set.
	Node(Polygon* arc) :  left(NULL), right(NULL), parent(NULL), l_poly(NULL), r_poly(NULL), edge(NULL), arc(arc) {}
	//Breakpoint between two arcs. Contains left and right polygons, as well as the left and right subtrees. Has a parent. No arc
	Node(Node* l, Node* r, Polygon* lp, Polygon* rp) : left(l), right(r), parent(parent), l_poly(lp), r_poly(rp), arc(NULL), edge(NULL) {}

	void insert(Polygon* arc);
	//True if Leaf, False if Inner
	bool check_leaf();
	bool check_inner();
};
//Main question:
//Given no nodes, what belongs in the root?
//How do I insert the first node
//How do I insert the second node\\\

//Given (a,b) and (c,d) are the foci of the parabola and yline is the directrix
VPoint* parabola_intersection(Node* node, double yline);

class Voronoi
{
public:
	double xlen, ylen;
	Node* root;

	vector<Edge*> edges;
	unordered_map<VPoint, Polygon*> polygons;
	unordered_map<VPoint, Vertex*> vertices;
	priority_queue<PQueue, vector<PQueue>, function<bool(PQueue, PQueue)>> event_queue;

	Voronoi(double x, double y) : xlen(x), ylen(y), polygons(), vertices(), edges(), root(new Node(new Polygon(new VPoint(x/2,-1* y)))), event_queue(QueueCompare) {}
	void tree_insert(Polygon* arc, double yline);
	void add_circle_event(Node* current_node);
};
