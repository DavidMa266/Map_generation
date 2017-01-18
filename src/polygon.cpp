#include "polygon.hpp"
#include <unordered_map>
#include <iostream>
#include <math.h>
#include <stdio.h>

void Edge::assign_opposite(Edge* opp)
{
	opposite = opp;
}

void Edge::assign_prev(Edge* p)
{
	prev = p;
}

void Edge::assign_next(Edge* n)
{
	next = n;
}

//Creates a leaf node
Node* new_node(Polygon* arc)
{
	Node* node = new Node();
	node->left = NULL;
	node->right = NULL;
	node->l_polygon = NULL;
	node->r_polygon = NULL;

	node->arc = arc;
	node->xy = arc->xy;
	return node;
}

//Creates a copy of a node, usually used for leaf nodes
Node* new_node(Node* n)
{
	Node* node = new Node();
	node->l_polygon = n->l_polygon;
	node->r_polygon = n->r_polygon;
	node->left 		= n->left;
	node->right 	= n->right;
	node->arc 		= n->arc;
	node->xy 		= n->xy;
	return node;
}


Node* new_node(Polygon* left_poly, Polygon* right_poly, Node* left, Node* right)
{
	Node* node = new Node();
	node->l_polygon = left_poly;
	node->r_polygon = right_poly;
	node->left = left;
	node->right = right;
	node->arc = NULL;
	node->xy = NULL;
	return node;
}


void compute_breakpoints(double _a, double _b, double _c, double _d, double yline)
{
/*	if(node->left != NULL){
		recompute_breakpoints(node->left, yline);
	}
	if(node->right != NULL)
	{
		recompute_breakpoints(node->right, yline);
	}*/
	double resx, resy;
	//Normalize coordinates so yline is essentailly '0'
/*	double a = node->l_polygon->xy->x;
	double b = yline - node->l_polygon->xy->y;

	double c = node->r_polygon->xy->x;
	double d = yline- node->r_polygon->xy->y;
*/
	double a,b,c,d;
	a = _a;
	b = yline-_b;
	c = _c;
	d = yline-_d;

	if(d == b)
		resx = (a + c) / 2;
	else if(d == yline)
		resx = c;
	else if(b == yline){
		resx = a;
	}
	else{
		double ax = d-b;
		double bx = -2 * (a*d - c*b);
		double cx = d * (a * a + b * b) - b * (c * c + d * d);
		resx = (-1 * bx + (sqrt(bx * bx - 4 * ax * cx))) / (2* ax);
	}
	cout << "X " << resx << endl;
}


//Whenever we insert, we must first iterate and update all breakpoint values.
void Tree::insert(Polygon* arc)
{
	Node* current_node = root;
	XY* xy = arc->xy;

	if(root == NULL)
	{
		root = new_node(arc);
	}
	else
	{
		//There should not be a case where one is non Null but the other is Null
		while(current_node->left != NULL || current_node->right != NULL)
		{
			if(xy->x < current_node->xy->x)
			{
				current_node = current_node->left;
			}
			else
			{
				current_node = current_node->right;
			}
		}
		//At this point the leaf node should be reached.

		//Create 3 nodes
		//One node with l_polygon = current_node-> arc
		//r_polygon = current arc

		//One node with l_polygon = current arc
		//r_polygon = current_node->arc;
		Node* prev = new_node(current_node);
		Node* leaf = new_node(arc);
		Node* next = new_node(current_node);

		Node* right_node = new_node(arc, next->arc, leaf, next);

		//Do this to transform current node to a inner node, without requireing me travel back up the tree and include a parent parameter.
		//If the parent parameter is ever introduced, this can go for a better method.
		current_node->left = prev;
		current_node->right = right_node;
		current_node->l_polygon = prev->arc;
		current_node->r_polygon = leaf->arc;
		current_node->arc = NULL;
		current_node->xy = NULL;
		//One leaf node 

	}
}

bool YCompare(XY xy1, XY xy2)
{
	return xy1.y < xy2.y;
}


int main()
{
	compute_breakpoints(3, 4, 1, 7, 8);
}