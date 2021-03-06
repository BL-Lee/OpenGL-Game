#include "Collision.h"
#include "../GLErrorCalls.h"
#include <iostream>
#include "../Renderer.h"
float Collision::closestDistance = closestDistance = FLT_MAX;
glm::vec2 Collision::closestNormal = { 0.0f,0.0f };;
int Collision::closestIndex = 0;

/*
* Returns the vertex closest to the direction given
*/
glm::vec2 Collision::getSupport(glm::vec2 shape[], int size, glm::vec2 direction)
{
	float mostPerpendicular = -FLT_MAX;
	glm::vec2 furthestVertex = glm::vec2(0.0f);

	for (int i = 0; i < size; i++)
	{
		float dot = glm::dot(shape[i], direction);
		if (dot > mostPerpendicular)
		{
			mostPerpendicular = dot;
			furthestVertex = shape[i];
		}
	}
 	ASSERT(furthestVertex.x != 0.0f);
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
	
	ASSERT(p.x != 0.0f || p.y != 0.0f);

	return p;
}
 /*
 * ab is a line segment
 */
glm::vec2 Collision::OrthoganalVectorTowardsTarget(glm::vec2 a, glm::vec2 b, glm::vec2 target)
{
	glm::vec2 line = b - a;

	glm::vec2 targetTowardsClockWise = (glm::vec2{ line.y, -line.x } + a) - target;
	glm::vec2 targetTowardsCounterClockWise = (glm::vec2{ -line.y, line.x } + a) - target;

	if (glm::dot(targetTowardsClockWise,targetTowardsClockWise) < glm::dot(targetTowardsCounterClockWise,targetTowardsCounterClockWise))
	{
		return glm::vec2{ line.y, -line.x };
	}
	else
	{
		return glm::vec2{ -line.y, line.x };
	}
}

void Collision::FindClosestPoint(glm::vec2 simplexVertices[], int size, Winding winding)
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
		norm = glm::normalize(norm);

		//this is also a projection, just norm is normalized
		float dist = glm::dot(norm, simplexVertices[i]);
		if (dist < closestDistance)
		{
			closestDistance = dist;
			closestIndex = j;
			closestNormal = norm;
		}
	}
}
/*
* Returns 0 vector if not colliding
*/
CollisionOverlapData Collision::IsColliding(glm::vec2 shapeA[], int sizeA, glm::vec2 shapeB[], int sizeB)
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
	glm::vec2 direction = glm::normalize(centerA - centerB);
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
				//glm::vec2 predirection = direction;
				direction = OrthoganalVectorTowardsTarget(simplexVertices[0], simplexVertices[1], { 0.0f,0.0f });
				//glm::vec2 d = TripleProduct(simplexVertices[1] - simplexVertices[0], -simplexVertices[0], simplexVertices[1] - simplexVertices[0]);
				//direction = glm::normalize(d);
				//ASSERT(direction.x < 2.0f);
				simplexVertices[2] = getSupport(shapeA, sizeA, direction) - getSupport(shapeB, sizeB, -direction);
				simplexCount++;
			}break;
			case 3:
			{
				//For readability
				glm::vec2 a = simplexVertices[2];
				glm::vec2 b = simplexVertices[1];
				glm::vec2 c = simplexVertices[0];

				glm::vec2 a0 = -a; //3rd vertex to origin
				glm::vec2 ab = b - a; //3rd vertex to 2nd vertex
				glm::vec2 ac = c - a; // 3rd vertex to 1st vertex

				glm::vec2 abPerp = -OrthoganalVectorTowardsTarget(a, b, c);//TripleProduct(ac, ab, ab); //direction away from third point on line from 
				glm::vec2 acPerp = -OrthoganalVectorTowardsTarget(a, c, b);//TripleProduct(ab, ac, ac);

				//Check if direction and a0 are pointing in the same direction, 
				//if they arent then we havent passed the origin, and we will never get an intersection
				if (glm::dot(-a0, direction) < 0)
				{
					return { glm::vec2(0), glm::vec2(0) };
				}
				if (glm::dot(abPerp, a0) > 0)
				{
					//origin is outside ab, get rid of c and go towards abPerp
					direction = glm::normalize(abPerp);
					//tester for now, remove 0 and add new support to index 2
					glm::vec2 temp = simplexVertices[1];
					simplexVertices[1] = simplexVertices[2];
					simplexVertices[0] = temp;
					simplexVertices[2] = getSupport(shapeA, sizeA, direction) - getSupport(shapeB, sizeB, -direction);
				}
				else if (glm::dot(acPerp, a0) > 0)
				{
					//origin outside ac, get rid of b
					direction = glm::normalize(acPerp);
					simplexVertices[1] = simplexVertices[2];
					simplexVertices[2] = getSupport(shapeA, sizeA, direction) - getSupport(shapeB, sizeB, -direction);
				}
				else
				{
					//origin inside both so its collided
					//First off gotta find the closest edge 
					//Some sort of polygon winding unexplained stuff?
					float e0 = (simplexVertices[1].x - simplexVertices[0].x) *
						(simplexVertices[1].y + simplexVertices[0].y);
					float e1 = (simplexVertices[2].x - simplexVertices[1].x) *
						(simplexVertices[2].y + simplexVertices[1].y);
					float e2 = (simplexVertices[0].x - simplexVertices[2].x) *
						(simplexVertices[0].y + simplexVertices[2].y);

					Winding winding = (e0 + e1 + e2) >= 0.0f
						? Winding::Clockwise : Winding::CntrClockwise;

					glm::vec2 intersection = {};

					while(simplexCount < COLLISION_MAX_WINDING - 2)
					{
						FindClosestPoint(simplexVertices, simplexCount, Winding::Clockwise);
						glm::vec2 support = getSupport(shapeA, sizeA, closestNormal) - getSupport(shapeB, sizeB, -closestNormal);
						float dist = glm::dot(closestNormal, support);//actually a projection, but since normal is normalized you can dot..
						
						intersection = closestNormal * dist;
						if (abs(dist - closestDistance) <= COLLISION_EPSILON)
						{
							return { intersection, closestNormal };
						}
						else
						{
							//insert
							for (int j = simplexCount; j > closestIndex; j--)
							{
								simplexVertices[j] = simplexVertices[j - 1];
							}
							simplexVertices[closestIndex] = support;
							simplexCount++;
						}
					}	
					return { intersection, closestNormal};
				}
			}break;
		}
	}
	//might need to change this later, if not colliding then dist is 0?
	return { glm::vec2(0), glm::vec2(0) };
}

float Collision::CrossProduct2D(glm::vec2 a, glm::vec2 b)
{
	return a.x * b.y - a.y * b.x;
}

glm::vec2 Collision::CrossProduct2D(float a, glm::vec2 b)
{
	return glm::vec2(-a * b.y, a * b.x);
}

void Collision::CorrectPosition(Entity& A, Entity& B, float penetrationDepth, glm::vec2 normal)
{
	float penetrationAllowance = 0.005f; // Penetration allowance
	float percentCorrection = 0.4f; // Penetration percentage to correct
	glm::vec2 correction = normal * (glm::max(penetrationDepth - penetrationAllowance, 0.0f) / (A.inverseMass + B.inverseMass) *  percentCorrection);
	A.pos -= glm::vec3{ correction.x, correction.y, 0.0f } * A.inverseMass;
	B.pos += glm::vec3{ correction.x, correction.y, 0.0f } * B.inverseMass;
}

void Collision::ResolveCollision(Entity& A, Entity& B, glm::vec2 collVector, glm::vec2 n)
{

	if (B.mass == 0.0f && A.mass == 0.0f) return; //both walls, dont move

	glm::vec2 contactA = collVector; //- glm::vec2{ A.pos.x, A.pos.y };
	glm::vec2 contactB = glm::vec2{ B.pos.x, B.pos.y } - (collVector + glm::vec2{ A.pos.x, A.pos.y });

	//Renderer::DrawLine({ A.pos.x, A.pos.y, A.pos.z - 0.01f }, { collVector.x + A.pos.x , collVector.y + A.pos.y , A.pos.z - 0.01f }, 2.0, { 1.0f,0.0f,1.0f,1.0f });
	//Renderer::DrawLine({ B.pos.x, B.pos.y, B.pos.z +0.01f }, { B.pos.x - collVector.x - A.pos.x , B.pos.y - collVector.y - A.pos.y , B.pos.z + 0.01f }, 2.0);

	//relative velocity
	glm::vec2 relV = (B.velocity + CrossProduct2D(B.angularVelocity, contactB)) -
		(A.velocity + CrossProduct2D(A.angularVelocity, contactA));

	//Renderer::DrawLine({ contactA, 0.0f }, { (n - contactA) * 10.0f, 0.0f });

	glm::vec2 normal = glm::normalize(n + contactA); 
	//projects the velocity along the normal
	//Assumes normal is unit vector
	float velAlongNormal = glm::dot(relV, normal);

	//only calculate impulse if they are moving apart
	if (velAlongNormal > 0.0f)
		return;

	/**TEMP TEMP TEMP 
	* CALCULATING INERTIA AS base^2*height^2/12
	* WHICH ONLY HOLDS TRUE FOR RECTANGLES
	**/


	float inertiaA = FLT_MAX;
	if (A.mass != 0.0f)
		inertiaA = A.size.x * A.size.x * A.size.y * A.size.y  * A.mass / 12;
	float inertiaB = FLT_MAX;
	if (B.mass != 0.0f)
		inertiaB = B.size.x * B.size.x * B.size.y * B.size.y  * B.mass / 12;

	float contactACrossNormal = CrossProduct2D(contactA, normal);
	float contactBCrossNormal = CrossProduct2D(contactB, normal);


	float usedRestitution = glm::min(A.restitution, B.restitution);
	float j = -(1 + usedRestitution) * velAlongNormal; 

	j /= A.inverseMass + B.inverseMass +
		(contactACrossNormal * contactACrossNormal / inertiaA) + 
		(contactBCrossNormal * contactBCrossNormal / inertiaB);
	//j /= A.inverseMass + B.inverseMass;
	//again projection, normal is unit vector
	glm::vec2 impulse = normal * j;

	A.velocity -= A.inverseMass * impulse;
	B.velocity += B.inverseMass * impulse;

	A.angularVelocity -= (1.0f / inertiaA) * CrossProduct2D(contactA, impulse);
	B.angularVelocity += (1.0f / inertiaB) * CrossProduct2D(contactB, impulse);

	float penetration = glm::length(contactA);

	CorrectPosition(A, B, penetration, normal);
	//friction part broken atm
#if 0
	relV = B.velocity - A.velocity;
	glm::vec2 tangent = relV - glm::dot(relV, n) * n;
	tangent = glm::normalize(tangent);
	float jt = -glm::dot(relV, tangent);

	jt /= A.inverseMass + B.inverseMass;
	float STATICFRICTION = 0.1;
	float mu = STATICFRICTION;

	glm::vec2 frictionImpulse;
	if (glm::abs(jt) < j * mu)
	{
		frictionImpulse = jt * tangent;
	}
	else
	{
		float DYNAMICFRICTION = 0.01;
		frictionImpulse = -j * tangent * DYNAMICFRICTION;
	}

	A.velocity -= A.inverseMass * frictionImpulse;
	B.velocity += B.inverseMass * frictionImpulse;
#endif
}
void Collision::ResolveCollisionNoRotation(Entity& A, Entity& B, glm::vec2 collVector, glm::vec2 n)
{
	glm::vec2 relV = B.velocity - A.velocity;
	
	//projects the velocity along the normal
	//Assumes normal is unit vector
	float velAlongNormal = glm::dot(relV, n);

	//only calculate impulse if they are moving apart
	if (velAlongNormal > 0.0f)
		return;

	float usedRestitution = glm::min(A.restitution, B.restitution);
	float j = -(1 + usedRestitution) * velAlongNormal;
	j /= A.inverseMass + B.inverseMass;
	//again projection, normal is unit vector
	glm::vec2 impulse = j * n;

	A.velocity -= A.inverseMass * impulse;
	B.velocity += B.inverseMass * impulse;
}

