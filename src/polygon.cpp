#include "polygon.hpp"
#include <unordered_map>
#include <iostream>

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

Tree::insert(Polygon* arc)
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


