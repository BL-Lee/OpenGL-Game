#pragma once
#include "glm/glm.hpp"
struct RigidBody
{
	float mass;
	float inverseMass;
	float restitution; //bounciness
	float rotation;
	glm::vec2 velocity;
	glm::vec2 convexHull[MAX_RIGIDBODY_HULL];
};