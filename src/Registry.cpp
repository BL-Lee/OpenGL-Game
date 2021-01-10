
#include "Registry.h"
#include "GLErrorCalls.h"
uint32_t Registry::idCounter = 0;
Entity* Registry::registry[MAX_ENTITIES_IN_REGISTRY] = { 0 };
uint8_t Registry::validityArray[MAX_ENTITIES_IN_REGISTRY] = { 0 };
uint32_t Registry::entityCount = 0;
std::vector<uint32_t> Registry::validIndices;
float Registry::MAX_LOAD_FACTOR = 0.75f;

uint32_t Registry::addEntity(Entity* e)
{
	if ((float)entityCount / MAX_ENTITIES_IN_REGISTRY > MAX_LOAD_FACTOR)
	{
		//resize
	}
	while (validityArray[idCounter])
	{
		idCounter = (idCounter + 1) % MAX_ENTITIES_IN_REGISTRY;
	}
	registry[idCounter] = e;
	validityArray[idCounter] = 1;
	entityCount++;
	validIndices.push_back(idCounter);
	e->id = idCounter;
	return idCounter;
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
		validIndices.erase(validIndices.begin() + indexOf(key));
		//do any cleanup for the entity, ex: freeing stuff
	}
}

uint32_t Registry::indexOf(uint32_t key)
{
	for (uint32_t i = 0; i < validIndices.size(); i++)
	{
		if (validityArray[i] == key)
			return i;
	}
	return -1;
}

