#include "Collision.h"
#include "../GLErrorCalls.h"
#include <iostream>
float Collision::closestDistance = closestDistance = FLT_MAX;
glm::vec2 Collision::closestNormal = { 0.0f,0.0f };;
int Collision::closestIndex = 0;

/*
* Returns the vertex closest to the direction given
*/
glm::vec2 Collision::getSupport(glm::vec2 shape[], int size, glm::vec2 direction)
{
	float mostPerpendicular = -FLT_MAX;
	glm::vec2 furthestVertex;

	for (int i = 0; i < size; i++)
	{
		float dot = glm::dot(shape[i], direction);
		if (dot > mostPerpendicular)
		{
			mostPerpendicular = dot;
			furthestVertex = shape[i];
		}
	}
	return furthestVertex;
}
/*
* Weird triple cross product that gives a perpendicular vector that likes to go towards the origin
* 
* no idea how it works
* middle term is the start point towards the point youw ant?
*/
glm::vec2 Collision::TripleProduct(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	glm::vec2 p;

	float ac = glm::dot(a, c); 
	float bc = glm::dot(b, c); 
	
	p.x = b.x * ac - a.x * bc;
	p.y = b.y * ac - a.y * bc;
	return p;
}

void Collision::FindClosestEdge(glm::vec2 simplexVertices[], int size, Winding winding)
{
	//eyy global data, might return a struct later
	closestDistance = FLT_MAX;
    closestNormal = { 0.0f,0.0f };
	closestIndex = 0;
	glm::vec2 edge = { 0.0f,0.0f };

	//Loop through each vertex
	for (int i = 0; i < size; i++)
	{
		int j = i + 1;
		if (j >= size) j = 0;
		//create an edge between two consecutive vertices
		edge = simplexVertices[j] - simplexVertices[i];
		glm::vec2 norm = {};
		switch (winding)
		{
			case Winding::Clockwise:
			{
				norm = { edge.y, -edge.x };
			}break;

			case Winding::CntrClockwise:
			{
				norm = { -edge.y, edge.x };
			}break;

			default:
			{
				ASSERT(0);
				std::cout << "Invalid Winding" << std::endl;
			}break;
		}
		glm::normalize(norm);

		float dist = glm::dot(norm, simplexVertices[i]);
		if (dist < closestDistance)
		{
			closestDistance = dist;
			closestNormal = norm;
			closestIndex = j;
		}
	}
}
/*
* Returns 0 vector if not colliding
*/
glm::vec2 Collision::isColliding(glm::vec2 shapeA[], int sizeA, glm::vec2 shapeB[], int sizeB)
{
	//Assumes center is just the center of all vertices, its okay for now
	glm::vec2 centerA = { 0.0f,0.0f };
	for (int i = 0; i < sizeA; i++)
	{
		centerA += shapeA[i];
	}
	centerA /= sizeA;
	glm::vec2 centerB = { 0.0f,0.0f };
	for (int i = 0; i < sizeB; i++)
	{
		centerB += shapeB[i];
	}
	centerB /= sizeB;

	glm::vec2 direction = centerA - centerB;
	glm::vec2 simplexVertices[COLLISION_MAX_WINDING];
	simplexVertices[0] = getSupport(shapeA, sizeA, direction) - getSupport(shapeB, sizeB, -direction);
	int simplexCount = 1;

	//0, unknown
	//1, contains
	//2, doesnt contain
	int containsOrigin = 0;

	while (!containsOrigin)
	{
		switch (simplexCount)
		{
			case 1:
			{
				//Create a new point in the opposite direction
				//Remember it will latch onto a vertex
				direction *= -1.0f;
				simplexVertices[1] = getSupport(shapeA, sizeA, direction) - getSupport(shapeB, sizeB, -direction);
				simplexCount++;
			}break;
			case 2:
			{
				//For the third vertex we should add it perpendicular to the line we made with the first two vertices towards the origin
#if 0
				glm::vec2 clockwise = { direction.y, -direction.x };
				
				float sqrDistClockwise = glm::dot(clockwise + simplexVertices[0], clockwise + simplexVertices[0]);
				float sqrDistCntrClockwise = glm::dot((-clockwise) + simplexVertices[0], (-clockwise) + simplexVertices[0]);

				if (sqrDistClockwise < sqrDistCntrClockwise)
					direction = clockwise;
				else
					direction = -clockwise;
				
				
				simplexVertices[2] = getSupport(shapeA, sizeA, direction) - getSupport(shapeB, sizeB, direction * -1.0f);
#endif
				direction = TripleProduct(simplexVertices[1] - simplexVertices[0], -simplexVertices[0], simplexVertices[1] - simplexVertices[0]);
				simplexVertices[2] = getSupport(shapeA, sizeA, direction) - getSupport(shapeB, sizeB, -direction);
				simplexCount++;
			}break;
			case 3:
			{
				//First check if it contains the origin

				//Check if the normal of the first two vertices (pointing away from the third vertex)
				//points in the same direction as the third vertex towards the origin

				
				//For readability
				glm::vec2 a = simplexVertices[2];
				glm::vec2 b = simplexVertices[1];
				glm::vec2 c = simplexVertices[0];

				glm::vec2 a0 = -a; //3rd vertex to origin
				glm::vec2 ab = b - a; //3rd vertex to 2nd vertex
				glm::vec2 ac = c - a; // 3rd vertex to 1st vertex

				glm::vec2 abPerp = TripleProduct(ac, ab, ab); //direction away from third point on line from 
				glm::vec2 acPerp = TripleProduct(ab, ac, ac);

				//Check if direction and a0 are pointing in the same direction, 
				//if they arent then we havent passed the origin, and we will never get an intersection
				if (glm::dot(-a0, direction) < 0)
				{
					return glm::vec2(0.0f, 0.0f);
				}


				if (glm::dot(abPerp, a0) > 0)
				{
					//origin is outside ab, get rid of c and go towards abPerp
					direction = abPerp;
					simplexVertices[0] = getSupport(shapeA, sizeA, direction) - getSupport(shapeB, sizeB, -direction);
				}
				else if (glm::dot(acPerp, a0) > 0)
				{
					//origin outside ac, get rid of b
					direction = acPerp;
					simplexVertices[1] = getSupport(shapeA, sizeA, direction) - getSupport(shapeB, sizeB, -direction);
				}
				else
				{
					//origin inside both so its collided
					//First off gotta find the closest edge 
					//Some sort of polygon winding unexplained stuff?
					float e0 = (simplexVertices[1].x - simplexVertices[0].x) *
						(simplexVertices[1].y + simplexVertices[1].y);
					float e1 = (simplexVertices[2].x - simplexVertices[1].x) *
						(simplexVertices[2].y + simplexVertices[1].y);
					float e2 = (simplexVertices[0].x - simplexVertices[2].x) *
						(simplexVertices[0].y + simplexVertices[1].y);

					Winding winding = (e0 + e1 + e2) >= 0.0f
						? Winding::Clockwise : Winding::CntrClockwise;

					glm::vec2 intersection = {};

					while(simplexCount < COLLISION_MAX_WINDING - 1)
					{
						FindClosestEdge(simplexVertices, simplexCount, Winding::Clockwise);
						glm::vec2 support = getSupport(shapeA, sizeA, closestNormal) - getSupport(shapeB, sizeB, -closestNormal);
						float dist = glm::dot(closestNormal, support);

						intersection = closestNormal * closestDistance;
						if (abs(dist - closestDistance) <= COLLISION_EPSILON)
						{
							return intersection;
						}
						else
						{
							simplexVertices[simplexCount++] = support;
						}
						if (simplexCount > sizeA || simplexCount > sizeB)
						{
							break;
						}
					}	
					return intersection;
				}
			}break;
		}
	}
	//might need to change this later, if not colliding then dist is 0?
	return glm::vec2(0.0f, 0.0f);
}
