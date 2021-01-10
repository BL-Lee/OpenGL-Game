#pragma once
#include "glm/glm.hpp"
#include <vector>
const int MAX_RIGIDBODY_HULL = 32;

struct Entity
{
	int id;

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
	glm::vec2 localVertices[MAX_RIGIDBODY_HULL];
	int vertexCount;
	glm::vec2 gravityDirection;
	float gravityStrength; //just use 0 for disabling

	//Movement
	void (*Accelerate)(Entity* e, float rot, float deltaTime);
	void (*UpdatePosition)(Entity* e, float deltaTime);

	//Visuals
	glm::vec4 colour;

};
void SetEntityMass(Entity* e, float m);
void AccelerateEntityForward(Entity* e, float rotation, float deltaTime);
void UpdatePosition(Entity* e, float deltaTime);
Entity* CloneEntity(Entity* e);
void UpdateWorldVertices(Entity* e);