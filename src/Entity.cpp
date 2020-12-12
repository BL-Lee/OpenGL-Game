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