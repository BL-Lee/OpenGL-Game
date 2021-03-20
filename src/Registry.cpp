
#include "Registry.h"
#include "GLErrorCalls.h"
uint64_t Registry::idCounter = 0;
uint32_t Registry::maxSize = 16;
Entity** Registry::registry = (Entity**)calloc(Registry::maxSize, sizeof(Entity*));
uint32_t* Registry::validityArray = (uint32_t*)calloc(Registry::maxSize, sizeof(uint32_t*));
uint32_t Registry::entityCount = 0;
std::vector<uint32_t> Registry::validIndices;
float Registry::MAX_LOAD_FACTOR = 0.75f;

uint32_t Registry::addEntity(Entity* e)
{
	if ((float)entityCount / Registry::maxSize > MAX_LOAD_FACTOR)
	{		
		#if 0
		registry = (Entity**)realloc(registry, sizeof(Entity*) * maxSize * 2);
		memset(registry + maxSize, 0, sizeof(Entity*) * maxSize);
		validityArray = (uint32_t*)realloc(validityArray, sizeof(uint32_t*) * maxSize * 2);
		memset(validityArray + maxSize, 0, sizeof(uint32_t*) * maxSize);
		maxSize *= 2;
#endif
		ASSERT(false); //TODO: rehash everything when resize
	}
	while (validityArray[idCounter % Registry::maxSize])
	{
		idCounter++;
	}
	registry[idCounter % Registry::maxSize] = e;
	validityArray[idCounter % Registry::maxSize] = 1;
	entityCount++;
	validIndices.push_back(idCounter);
	e->id = idCounter;
	printf("Entity added: %d slot: %d\n", idCounter, idCounter % Registry::maxSize);
	return idCounter;
}

Entity* Registry::getEntityByKey(uint32_t id)
{
	if (!validityArray[id % Registry::maxSize])
		return NULL;
	if (registry[id % Registry::maxSize]->id == id)
		return registry[id % Registry::maxSize];
	
	return NULL;
}

Entity* Registry::cloneEntity(Entity* e)
{	
	return cloneEntity(e->id);
}
Entity* Registry::cloneEntity(uint32_t id)
{
	Entity* e = getEntityByKey(id);
	Entity* newEntity = CloneEntity(e);
	addEntity(newEntity);
	return newEntity;
}

void Registry::deleteEntity(uint32_t id)
{
	ASSERT(validityArray[id % Registry::maxSize]) //deleting an already deleted entity
	if (validityArray[id % Registry::maxSize])
	{
		bool deleted = false;
		for (int i = 0; i < validIndices.size(); i++)
		{
			if (validIndices[i] == id)
			{
				validIndices.erase(validIndices.begin() + i);
				deleted = true;
				break;
			}
		}
		ASSERT(deleted);
		//do any cleanup for the entity, ex: freeing stuff
		Entity* e = getEntityByKey(id);
		validityArray[id % Registry::maxSize] = 0;
		free(e);
		entityCount--;
	}
}
/*
uint32_t Registry::indexOf(uint32_t key)
{
	for (uint32_t i = 0; i < validIndices.size(); i++)
	{
		if (validityArray[i] == key % Registry::maxSize)
			return i;
	}
	return -1;
}

*/