
#include "Registry.h"
#include "GLErrorCalls.h"
uint32_t Registry::idCounter = 0;
uint32_t Registry::maxSize = 32;
Entity** Registry::registry = (Entity**)calloc(Registry::maxSize, sizeof(Entity*));
uint32_t* Registry::validityArray = (uint32_t*)calloc(Registry::maxSize, sizeof(uint32_t*));
uint32_t Registry::entityCount = 0;
std::vector<uint32_t> Registry::validIndices;
float Registry::MAX_LOAD_FACTOR = 0.75f;

uint32_t Registry::addEntity(Entity* e)
{
	if ((float)entityCount / Registry::maxSize > MAX_LOAD_FACTOR)
	{		
		registry = (Entity**)realloc(registry, sizeof(Entity*) * maxSize * 2);
		memset(registry + maxSize, 0, sizeof(Entity*) * maxSize);
		validityArray = (uint32_t*)realloc(validityArray, sizeof(uint32_t*) * maxSize * 2);
		memset(validityArray + maxSize, 0, sizeof(uint32_t*) * maxSize);
		maxSize *= 2;
	}
	while (validityArray[idCounter])
	{
		idCounter = (idCounter + 1) % Registry::maxSize;
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
		return NULL;
	return registry[key];
}

Entity* Registry::cloneEntity(Entity* e)
{	
	return cloneEntity(e->id);
}
Entity* Registry::cloneEntity(uint32_t key)
{
	Entity* e = getEntityByKey(key);
	Entity* newEntity = CloneEntity(e);
	addEntity(newEntity);
	return newEntity;
}

void Registry::deleteEntity(uint32_t key)
{
	ASSERT(validityArray[key]) //deleting an already deleted entity
	if (validityArray[key])
	{
		bool deleted = false;
		for (int i = 0; i < validIndices.size(); i++)
		{
			if (validIndices[i] == key)
			{
				validIndices.erase(validIndices.begin() + i);
				deleted = true;
				break;
			}
		}
		ASSERT(deleted);
		//do any cleanup for the entity, ex: freeing stuff
		Entity* e = getEntityByKey(key);
		validityArray[key] = 0;
		free(e);
		entityCount--;
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

