
#include "Registry.h"
#include "GLErrorCalls.h"
uint32_t Registry::idCounter = 0;
Entity* Registry::registry[MAX_ENTITIES_IN_REGISTRY] = { 0 };
uint8_t Registry::validityArray[MAX_ENTITIES_IN_REGISTRY] = { 0 };
uint32_t Registry::entityCount = 0;
float Registry::MAX_LOAD_FACTOR = 0.75f;

uint32_t Registry::addEntity(Entity* e)
{
	if ((float)entityCount / MAX_ENTITIES_IN_REGISTRY > MAX_LOAD_FACTOR)
	{
		//resize
	}
	while (validityArray[idCounter % MAX_ENTITIES_IN_REGISTRY])
	{
		idCounter++;
	}
	registry[idCounter % MAX_ENTITIES_IN_REGISTRY] = e;
	validityArray[idCounter % MAX_ENTITIES_IN_REGISTRY] = 1;
	entityCount++;
	return idCounter % MAX_ENTITIES_IN_REGISTRY;
}

Entity* Registry::getEntityByKey(uint32_t key)
{
	if (!validityArray[key])
		return nullptr;
	return registry[key];
}

void Registry::deleteEntity(uint32_t key)
{
	ASSERT(!validityArray[key]) //deleting an already deleted entity
	if (validityArray[key])
	{
		validityArray[key] = 0;
		//do any cleanup for the entity, ex: freeing stuff
	}
}

