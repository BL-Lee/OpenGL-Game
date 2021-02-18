#pragma once
#include "glm/glm.hpp"
#include "../Entity.h"
/**
* TODO(Ben): Broad phase collision detection, probably AABB
*			 maybe some spatial tree to optimize broad phase
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
*	3. If they are colliding (contains origin), the closest point from the origin to the hull of the sum is the
*		penetration vector. ie how much it is colliding by
*/

/*
* 
*/
struct CollisionOverlapData
{
	glm::vec2 AToCollisionPoint;
	glm::vec2 normalOffB;
};
struct Manifold
{

};
class Collision
{
private:
	static float closestDistance;
	static glm::vec2 closestNormal;
	static int closestIndex;
	static constexpr float COLLISION_EPSILON = 0.001f;
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
	
	static void FindClosestPoint(glm::vec2 simplexVertices[], int size, Winding winding);
	static float CrossProduct2D(glm::vec2 a, glm::vec2 b);
	static glm::vec2 CrossProduct2D(float a, glm::vec2 b);
	static void CorrectPosition(Entity& A, Entity& B, float penetrationDepth, glm::vec2 normal);
	
public:
	static CollisionOverlapData IsColliding(glm::vec2 shapea[], int sizea, glm::vec2 shapeb[], int sizeb);
	static void ResolveCollision(Entity& A, Entity& B, glm::vec2 collVector, glm::vec2 normal);
	static void ResolveCollisionNoRotation(Entity& A, Entity& B, glm::vec2 collVector, glm::vec2 normal);
	static glm::vec2 OrthoganalVectorTowardsTarget(glm::vec2 a,glm::vec2 b, glm::vec2 target);
	
};