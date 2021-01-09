#include "Entity.h"

void AccelerateEntityForward(Entity* e, float rotation, float deltaTime)
{
	glm::vec2 direction = { cos(rotation), sin(rotation) };
	e->velocity += direction * e->acceleration * deltaTime;
}
void UpdatePosition(Entity* e, float deltaTime)
{
	e->pos.x += e->velocity.x * deltaTime;
	e->pos.y += e->velocity.y * deltaTime;
	//e->pos += e->gravityDirection * e->gravityStrength * deltaTime;
	e->velocity += e->gravityDirection * e->gravityStrength * deltaTime;
}
void SetEntityMass(Entity* e, float m)
{
	e->mass = m;
	if (m == 0.0f)
		e->inverseMass = 0.0f;
	else
		e->inverseMass = 1 / m;
}
Entity* CloneEntity(Entity* e)
{
	Entity* newEntity = (Entity*)malloc(sizeof(Entity)); 
	memcpy(newEntity, e, sizeof(Entity));//Not sure if this will work with func pointers
	return newEntity;
}