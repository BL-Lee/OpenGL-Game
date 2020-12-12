#include "Collision.h"


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

bool Collision::isColliding(glm::vec2 shapeA[], int sizeA, glm::vec2 shapeB[], int sizeB)
{
	//Ideally we would choose the point between the two centers, add later
	glm::vec2 direction = { 0.0f,1.0f };
	glm::vec2 simplexVertices[3];
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
					return false;
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
					return true;
				}


			}break;
		}
	}
}
