#pragma once
#include "Entity.h"
const 
class Registry
{
public:
	//Counter is used to insert, incremented each insert
	static uint64_t idCounter;
	static uint32_t maxSize;
	static Entity** registry;
	static uint32_t* validityArray;
	static std::vector<uint32_t> validIndices;
	static float MAX_LOAD_FACTOR;
	static uint32_t entityCount;
	//Keep a dynamic data structure that contains the indices of all valid entities?
	//for looping over all entities
	static Entity* getEntityByKey(uint32_t key);

	static uint32_t addEntity(Entity* value);
	static void deleteEntity(uint32_t key);
	static Entity* cloneEntity(Entity* e);
	static Entity* cloneEntity(uint32_t key);
private:
	static uint32_t indexOf(uint32_t key);
};