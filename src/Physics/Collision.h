#pragma once
#include "glm/glm.hpp"
/**
* TODO(Ben): Broad phase collision detection, probably AABB
* Narrow phase collision is calculated using the Gilbert-Johnsohn-Keerthi (GJK) collision detection
* Only works for convex shapes
*/

/**
* Main Idea
*	1. Use minkowski difference to "add" the shapes together
*	2. Check if the added shape includes the origin
*	3. Profit
* Actual method
*	1. Instead of calculating the whole minkowski difference, 
*		we'll use support functions to find the furthest vertex on a shape in a given direction
*	2. Create 'simplexes' (In 2d it will just be a triangle) that are made from the vertices of the sum,
*		if the triangle contains the origin we stop and say it's colliding
*		if not we make a new simplex in the opposite direction until we're sure the origin isnt contained
*/

/*
* Main colliding function for narrow phase,
* Assumes 2D, and that 
*/
class Collision
{
private:
	static float closestDistance;
	static glm::vec2 closestNormal;
	static int closestIndex;
	static constexpr float COLLISION_EPSILON = 0.1f;
	static const int COLLISION_MAX_WINDING = 32;
	enum Winding
	{
		Clockwise = 0,
		CntrClockwise = 1
	};
	static int sizeA;
	static int sizeB;
	static glm::vec2 getSupport(glm::vec2 shape[], int size, glm::vec2 direction);
	static glm::vec2 TripleProduct(glm::vec2 a, glm::vec2 b, glm::vec2 c);
	static void FindClosestEdge(glm::vec2 simplexVertices[], int size, Winding winding);
public:
	static glm::vec2 isColliding(glm::vec2 shapea[], int sizea, glm::vec2 shapeb[], int sizeb);
};