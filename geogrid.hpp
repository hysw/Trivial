// Copyright by no one: just throw the useless copyright away...
// an implementation of Geodesic Grid by hysw
// Peter Chen (peterctz2009@gmail.com)
// work under CC0 (Public Domain)

#pragma once

#include <vector>
#include <cmath>
using namespace std;

namespace hysw{namespace geogrid{
  struct Node{
	public:
		int id,neighbor[6];
		// position
		double x,y,z;
	};
	std::vector<Node> GenerateGrid(int iteration);
}}
