#include "fortune_line_sweep.hpp"

#include <functional>
#include <queue>
#include <vector>
#include <iostream>


handle_event(XY xy, Map* map, Tree* tree)
{
	
}

Map* line_sweep(vector<XY*> centers, int iteration)
{
	Map* map = new Map();
	priority_queue<XY, vector<XY>, function<bool(XY, XY)>> event_queue(YCompare);

	Tree* beach_line;
	
	for( XY* &xy: centers)
	{
		event_queue.push(*xy);
		map->polygons.insert({*xy, new Polygon(xy)});
	}

	//Iterate through the priority queue and pull events off
	//Create the tree


	//Whenever we add a new event, we shove it onto the event queue.


	//Remake new centers.
/*	if(iteration > 0)
	{
		return line_sweep(centers, iteration-1);
	}
*/
	return new Map(); 
}
