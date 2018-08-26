#pragma once

#include "Edge.h"

#include <memory>

class QuadEdge
{
public:
	Edge edges[4];
	QuadEdge();
};

QuadEdge::QuadEdge()
{
	edges[0].setIndex(0);
	edges[1].setIndex(1);
	edges[2].setIndex(2);
	edges[3].setIndex(3);

	edges[0].draw = false;
	edges[1].draw = false;
	edges[2].draw = false;
	edges[3].draw = false;

	edges[0].setNext((edges + 0));
	edges[1].setNext((edges + 3));
	edges[2].setNext((edges + 2));
	edges[3].setNext((edges + 1));
}

Edge* Edge::Make(std::vector<QuadEdge*>& list)
{
	list.push_back(new QuadEdge());
	return list.back()->edges;
}