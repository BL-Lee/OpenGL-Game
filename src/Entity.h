#pragma once
#include "glm/glm.hpp"
#include <vector>
const int MAX_RIGIDBODY_HULL = 32;

struct Entity
{
	//Transformation
	glm::vec3 pos;
	glm::vec2 size;
	glm::vec2 velocity;
	float rotation;
	float acceleration;
	
	//RigidBody
	float mass;
	float inverseMass;
	void (*SetMass)(Entity& e, float m);
	float restitution; //bounciness
	glm::vec2 vertices[MAX_RIGIDBODY_HULL];
	int vertexCount;

	//Movement
	void (*Accelerate)(Entity& e, float rot);
	void (*UpdatePosition)(Entity& e);

	//Visuals
	glm::vec4 colour;

};
void SetEntityMass(Entity& e, float m);
void AccelerateEntityForward(Entity& e, float rotation);
void UpdatePosition(Entity& e);