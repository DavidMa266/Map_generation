#include "voronoi.hpp"
#include <math.h>
#include <iostream>
#include <functional>



bool Node::check_leaf(){return (this->arc != NULL && this->l_poly == NULL && this->r_poly == NULL && this->left == NULL && this->right == NULL && this->edge == NULL);}
bool Node::check_inner(){return (this->arc == NULL && this->l_poly != NULL && this->r_poly != NULL && this->left != NULL && this->right != NULL && this->edge != NULL);}
double euclidean_distance(VPoint* center, VPoint* any) { return sqrt((center->x - any->x) * (center->x - any->x) + (center->y - any->y) * (center->y - any->y));}

bool QueueCompare(PQueue pq1, PQueue pq2){ return pq1.point->y < pq2.point->y;}

//n1 is this, n2 is inner
//Create two edges for two inner nodes.
void create_edges(Node* n1, Node* n2)
{
	//Both must be inner nodes
	Polygon* p1 = n1->l_poly;
	Polygon* p2 = n1->r_poly;

	Edge* e1 = new Edge(p1, p2);
	Edge* e2 = new Edge(p2, p1);

	p1->edges.push_back(e1);
	p2->edges.push_back(e2);

	e1->opposite = e2;
	e2->opposite = e1;

	n1->edge = e1;
	n2->edge = e2;
}

/* 
Function should only be run on leaf nodes.
We have the structre as follows
		this


And we want to turn it into 
		  this
	left		inner
		   center  	right

Left and Right point to teh same Polygon
Center points to the new arc

Inner has its left polygon point to arc, and 

*/

void Node::insert(Polygon* arc)
{
	Polygon* old_arc = this->arc;

	Node* left = new Node(old_arc);
	Node* center = new Node(arc);
	Node* right = new Node(old_arc);

	Node* inner = new Node(center, right, arc, old_arc);

	left->parent = this;
	center->parent = inner;
	right->parent = inner;
	inner->parent = this;

	this->left = left;
	this->right = inner;
	this->l_poly = old_arc;
	this->r_poly = arc;
	this->arc = NULL;

	create_edges(this, inner);
}


VPoint* parabola_intersection(double a, double _b, double c, double _d, double yline)
{
	double b,d, resx, resy;
	b = yline - _b;
	d = yline - _d;
	if(d == b){
		resx = (a + c) / 2;
		resy = ((resx - a)*(resx-a) + _b*_b - yline* yline)/(2*(_b- yline));
	}
	else if(_d == yline){
		resx = c;
		resy = ((resx - a)*(resx-a) + _b*_b - yline* yline)/(2*(_b- yline));
	}
	else if(_b == yline){
		resx = a;
		resy = ((resx - c)*(resx-c) + _d*_d - yline* yline)/(2*(_d- yline));
	}
	else{
		double ax = d-b;
		double bx = -2 * (a*d - c*b);
		double cx = d * (a * a + b * b) - b * (c * c + d * d);
		resx = (-1 * bx + (sqrt(bx * bx - 4 * ax * cx))) / (2* ax);
		resy = ((resx - a)*(resx-a) + _b*_b - yline* yline)/(2*(_b- yline));
	}
	cout << "A " << a << " B " << b << " C " << c << " D " << d << endl;
	cout << "RESX " << resx <<endl;
	return new VPoint(resx,resy);
}

VPoint* parabola_intersection(Polygon* left, Polygon* right, double yline)
{
	return parabola_intersection(left->xy->x, left->xy->y, right->xy->x, right->xy->y, yline);
}

//Expects an inner node
//Returns the breakpoint intersection
VPoint* parabola_intersection(Node* node, double yline)
{
	return parabola_intersection(node->l_poly, node->r_poly, yline);
}

double slope(double a, double b, double c, double d)
{
	return (d-b)/(c-a);
}


VPoint* midpoint(double a, double b, double c, double d)
{
	return new VPoint((a+c)/2, (b+d)/2);
}
//Just return any negative value if the points are all collinear
VPoint* find_circle_event(double a, double b, double c, double d, double e, double f)
{
	//Collinear, fail it
	if( ((a==c) && (c==e)) || ((b==d) && (b == f))){
		return NULL;
	}


	VPoint* mdpt1 = midpoint(a,b,c,d);
	VPoint* mdpt2 = midpoint(c,d,e,f);

	bool vertical_line_1, vertical_line_2 = false;
	double centerx, centery;
	double slope1, slope2;
	

	if(a == c)
	{
		slope2 = slope(c,d,e,f);
		centery = mdpt1->y;
		vertical_line_1 = true;
	}
	else if(c == e)
	{
		slope1 = slope(a,b,c,d);		
		centery = mdpt2->y;
		vertical_line_2 = true;
	}
	else{
		slope1 = slope(a,b,c,d);
		slope2 = slope(c,d,e,f);
	}

	double _a, _b, _c, _d;
	_a = mdpt1->x;
	_b = mdpt1->y;
	_c = mdpt2->x;
	_d = mdpt2->y;

	if(vertical_line_1)
	{
		centerx = (-1 * slope2) * (centery - _d) + _c;
	}
	else if(vertical_line_2)
	{
		centerx = (-1 * slope1) * (centery - _b) + _a;
	}
	else if(slope2 == slope1){
		return NULL;
	}
	else{
		centerx = (slope1 * slope2 *(b - f) + slope2 * (a + c) - slope1 * (c + e))/(2*(slope2-slope1));
		if(slope1 == 0)
			centery = (-1/slope2) * (centerx - _c) + _d;
		else
			centery = (-1/slope1) * (centerx - _a) + _b;	
	}
	return new VPoint(centerx, centery);
	//Find perpindicular bisectors of 2 points
	//Do it again
	//Then solve the equation
}


VPoint* find_circle_event(Polygon* arc1, Polygon* arc2, Polygon* arc3)
{
	return find_circle_event(arc1->xy->x, arc1->xy->y, arc2->xy->x, arc2->xy->y, arc3->xy->x, arc3->xy->y);
}

//We expect the node to be the inner node of the node we just replaced.
VPoint* find_left_circle_event(Node* old_node)
{
	//How do I find the relevant arcs?
	//I have arc2 and arc1/3 definetly. But the last case might depend.
	Polygon* arc1, *arc2, *arc3;
	//Arc1 should be old_node->r_poly
	arc1 = old_node->r_poly;
	arc2 = old_node->l_poly;
	Node* current = old_node->parent;
	while(current != NULL)
	{
		//keep traveling up the bloody tree until we find a situation where l_poly != old_node
		if(current->l_poly != arc1)
		{
			arc3 = current->l_poly;
			return find_circle_event(arc1,arc2,arc3);
		}
		current = current->parent;
	}
	return NULL;
}

VPoint* find_right_circle_event(Node* old_node)
{
	Polygon* arc1, *arc2, *arc3;
	arc1 = old_node->r_poly;
	arc2 = old_node->l_poly;
	Node* current = old_node->parent;
	while(current != NULL)
	{
		//keep traveling up the bloody tree until we find a situation where r_poly != old_node
		if(current->r_poly != arc1)
		{
			arc3 = current->l_poly;
			return find_circle_event(arc1,arc2,arc3);
		}
		current = current->parent;
	}
	return NULL;
}



void Voronoi::add_circle_event(Node* current_node)
{
	VPoint* left_center = find_left_circle_event(current_node);
	VPoint* right_center = find_right_circle_event(current_node); 

	VPoint* arc = current_node->r_poly->xy;
	double radius;


	if(left_center != NULL)
	{
		radius = euclidean_distance(left_center, arc);
		this->event_queue.push(PQueue(new VPoint(left_center->x, left_center->y + radius), radius));
	}

	if(right_center != NULL)
	{
		radius = euclidean_distance(right_center, arc);
		this->event_queue.push(PQueue(new VPoint(right_center->x, right_center->y + radius), radius));
	}
	//Have the center, find the lowest point
}




void print_tree(Node * root, int level)
{
	if(root->left != NULL && root->right != NULL)
	{
		print_tree(root->left, level + 1);
		
		for (int i = 0; i < level; i++)
		{
			cout << "\t" ;
		}
		
		cout << "I: "<< root->l_poly->xy->x << " " << root->l_poly->xy->y << " " << root->r_poly->xy->x << " " << root->r_poly->xy->y <<endl;
		print_tree(root->right, level+ 1);
	}
	else{

		for (int i = 0; i < level; ++i)
		{
			cout << "\t" ;
		}
		
		cout << "L: " <<root->arc->xy->x << " "<< root->arc->xy->y << endl;
	}
}


void Voronoi::tree_insert(Polygon* arc, double yline)
{
	//All this function needs to do is find the correct leaf node to replace.

	Node* current_node = root;
	while(current_node->check_inner())
	{
		VPoint* intersection = parabola_intersection(current_node, yline);
		cout << "INTERSECTION POINT " << intersection->x << " " << intersection->y<< endl;
		cout << "ARC POINT " << arc->xy->x << " " << arc->xy->y << endl;
		cout << "CURRENT NODE LEFT " << current_node->l_poly->xy->x << " " << current_node->l_poly->xy->y << endl;
		cout << "CURRENT NODE RIGHT " << current_node->r_poly->xy->x << " " << current_node->r_poly->xy->y << endl;
		if(intersection->x >= arc->xy->x)
		{
			current_node = current_node->left;
		}
		else
		{
			current_node = current_node->right;
		}
	}
	current_node->insert(arc);
	this->add_circle_event(current_node);
}



void test()
{

	double x = 100;
	double y = 100;
	Voronoi* v = new Voronoi(x, y);

	double yline = 10;

	VPoint* point = new VPoint(30, 10);
	Polygon* poly = new Polygon(point);
	v->tree_insert(poly, yline);


	print_tree(v->root, 0);
	point = new VPoint(70, 10);
	poly = new Polygon(point);

	v->tree_insert(poly, yline);
	
	print_tree(v->root, 0);
	yline = 40;
	point = new VPoint(30, 40);
	poly = new Polygon(point);
	v->tree_insert(poly, yline);

	print_tree(v->root, 0);

	yline = 45;

	point = new VPoint(60, 45);
	poly = new Polygon(point);
	v->tree_insert(poly, yline);

	print_tree(v->root, 0);
}

int main()
{
	test();
}