#include "Entity.h"
#include "glm/gtc/matrix_transform.hpp"
void AccelerateEntityForward(Entity* e, float rotation, float deltaTime)
{
	glm::vec2 direction = { cos(rotation), sin(rotation) };
	e->velocity += direction * e->acceleration * deltaTime;
}
void UpdateTransform(Entity* e, float deltaTime)
{
	e->pos.x += e->velocity.x * deltaTime;
	e->pos.y += e->velocity.y * deltaTime;
	e->rotation += e->angularVelocity * deltaTime;
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
void SetEntityAABB(Entity* e)
{
	float maxDistToCorner = glm::sqrt(e->size.x * e->size.x + e->size.y * e->size.y);
	e->AABBHeight = maxDistToCorner;
	e->AABBWidth = maxDistToCorner;
}
Entity* CloneEntity(Entity* e)
{
	Entity* newEntity = (Entity*)malloc(sizeof(Entity)); 
	memcpy(newEntity, e, sizeof(Entity));//Not sure if this will work with func pointers
	return newEntity;
}
void UpdateWorldVertices(Entity* e)
{
	glm::mat4 result;
	glm::mat4 identity = glm::identity<glm::mat4>(); 
	//for the love of god make a math library, 
	//it's in 2d so you're always rotating around Z axis,
	//never scaling in z direction
	//you could do this with a mat3 but glm doesnt support that
	result = glm::translate(identity, e->pos)
		* glm::rotate(identity, e->rotation, { 0.0f,0.0f,1.0f })
		* glm::scale(identity, { e->size.x,e->size.y,1.0f });
	for (int i = 0; i < e->vertexCount; i++)
	{
		e->vertices[i] = result * glm::vec4{ e->localVertices[i].x, e->localVertices[i].y, 0.0f, 1.0f };
	}
}
void CopyVertices(Entity* e, glm::vec2* vertices, int count)
{
	memcpy(e->localVertices, vertices, sizeof(glm::vec2) * count);
	e->vertexCount = count;
}