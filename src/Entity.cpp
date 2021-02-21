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
	e->velocity += e->gravityDirection * deltaTime;
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
	//for the love of god make a math library, 
	//it's in 2d so you're always rotating around Z axis,
	//never scaling in z direction
	//you could do this with a mat3 but glm doesnt support that
	glm::mat4 transform = glm::identity<glm::mat4>();

	double sinRot = sin(e->rotation);
	double cosRot = cos(e->rotation);

	transform[0][0] = e->size.x * cosRot;
	transform[1][0] = e->size.y * -sinRot;
	transform[3][0] = e->pos.x;
	transform[0][1] = e->size.x * sinRot;
	transform[1][1] = e->size.y * cosRot;
	transform[3][1] = e->pos.y;
	transform[3][2] = e->pos.z;

	for (int i = 0; i < e->vertexCount; i++)
	{
		e->vertices[i] = transform * glm::vec4{ e->localVertices[i].x, e->localVertices[i].y, 0.0f, 1.0f };
	}
}
void CopyVertices(Entity* e, glm::vec2* vertices, int count)
{
	memcpy(e->localVertices, vertices, sizeof(glm::vec2) * count);
	e->vertexCount = count;
}
void UpdateTransformationMatrix(Entity* e)
{
	glm::mat4 transform = glm::identity<glm::mat4>();

	double sinRot = sin(e->rotation);
	double cosRot = cos(e->rotation);

	transform[0][0] = e->size.x * cosRot;
	transform[1][0] = e->size.y * -sinRot;
	//transform[2][0] = 0;
	transform[3][0] = e->pos.x;

	transform[0][1] = e->size.x * sinRot;
	transform[1][1] = e->size.y * cosRot;
	//transform[2][1] = 0;
	transform[3][1] = e->pos.y;

	//transform[0][2] = 0;
	//transform[1][2] = 0;
	//transform[2][2] = 1;
	transform[3][2] = e->pos.z;

	//transform[0][3] = 0;
	//transform[1][3] = 0;
	//transform[2][3] = 0;
	//transform[3][3] = 1;

	e->transformationMatrix = transform;
}