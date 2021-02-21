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
	float angularVelocity;
	float acceleration;
	glm::mat4 transformationMatrix;
	
	//RigidBody
	float mass;
	float inverseMass;
	float inertia;
	float inverseInertia;

	float AABBWidth;
	float AABBHeight;

	void (*SetMass)(Entity& e, float m);
	float restitution; //bounciness
	glm::vec2 vertices[MAX_RIGIDBODY_HULL];
	glm::vec2 localVertices[MAX_RIGIDBODY_HULL];
	int vertexCount;
	glm::vec2 gravityDirection;

	//Movement
	void (*Accelerate)(Entity* e, float rot, float deltaTime);
	void (*UpdateTransform)(Entity* e, float deltaTime);

	//Visuals
	glm::vec4 colour;
};
void SetEntityMass(Entity* e, float m);
void AccelerateEntityForward(Entity* e, float rotation, float deltaTime);
void UpdateTransform(Entity* e, float deltaTime);
Entity* CloneEntity(Entity* e);
void UpdateWorldVertices(Entity* e);
void SetEntityAABB(Entity* e);
void CopyVertices(Entity* e, glm::vec2* vertices, int count);
void UpdateTransformationMatrix(Entity* e);