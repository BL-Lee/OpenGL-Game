#pragma once
#include "glm/glm.hpp"
#include <vector>
//Entity system is a biiiit clunky rn
//doing player.accelerate(player); is a bit weird no?
//suppose each accelerate function could be different for each entity
//maybe see if theres a way for it to implicitly take itself as a parameter
//	I guess member functions
		//Edit: nope gotta pass it
struct Entity
{
	//Transformation
	glm::vec3 pos;
	glm::vec2 size;
	glm::vec2 velocity;
	float rotation;
	float acceleration;
	glm::vec2* vertices;

	//Movement
	void (*Accelerate)(Entity& e, float rot);
	void (*UpdatePosition)(Entity& e);

	//Collision
	std::vector<glm::vec2> convexHull;

	//Visuals
	glm::vec4 colour;

};
void AccelerateEntityForward(Entity& e, float rotation);
void UpdatePosition(Entity& e);