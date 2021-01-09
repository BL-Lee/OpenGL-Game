#pragma once
#include "Entity.h"
const int MAX_ENTITIES_IN_REGISTRY = 256;
class Registry
{
public:
	//Counter is used to insert, incremented each insert
	static uint32_t idCounter;
	static Entity* registry[MAX_ENTITIES_IN_REGISTRY];
	static uint8_t validityArray[MAX_ENTITIES_IN_REGISTRY];
	static std::vector<uint32_t> validIndices;
	static float MAX_LOAD_FACTOR;
	static uint32_t entityCount;
	//Keep a dynamic data structure that contains the indices of all valid entities?
	//for looping over all entities
	static Entity* getEntityByKey(uint32_t key);
	//Returns the index in the registry, to be used as key
	//Because counter is incremented each time, just use %tablesize as hash function
	//use linear probing?
	static uint32_t addEntity(Entity* value);
	static void deleteEntity(uint32_t key);
private:
	static uint32_t indexOf(uint32_t key);
};