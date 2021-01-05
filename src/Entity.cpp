#include "Entity.h"

void AccelerateEntityForward(Entity& e, float rotation)
{
	glm::vec2 direction = { cos(rotation), sin(rotation) };
	e.velocity += direction * e.acceleration;
}
void UpdatePosition(Entity& e)
{
	e.pos.x += e.velocity.x;
	e.pos.y += e.velocity.y;
}
void SetEntityMass(Entity& e, float m)
{
	e.mass = m;
	if (m == 0.0f)
		e.inverseMass = 0.0f;
	else
		e.inverseMass = 1 / m;
}