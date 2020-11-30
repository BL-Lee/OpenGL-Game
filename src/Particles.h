#pragma once

#include "Renderer.h"
#include "glm/glm.hpp"
#include <vector>
#include "OrthoCamera.h"
#include "Random.h"
struct Particle
{
	glm::vec2 Pos = { 0.0f,0.0f };
	glm::vec2 Size = { 0.0f, 0.0f };
	glm::vec2 EndSize = { 0.10f, 0.10f };
	glm::vec4 StartColour = { 0.0f, 0.4f, 0.9f, 1.0f };
	glm::vec4 EndColour = { 0.9f,0.5f,0.0f,1.0f };
	glm::vec2 Velocity = { 0.0f, 0.0f };
	float Rotation = Random::Float() * 360.0f;
	float RotationDelta = Random::Float() * 10.0f;
	float Lifetime = 1.0f;
	float CurrentLifetime = 0.0f;
	bool IsActive = true;
};
class ParticleSystem
{
private:
	static Particle s_BaseParticle;
	static std::vector<Particle> s_ParticlePool;
	static uint32_t s_TotalParticles;
	static uint32_t s_ParticleIndex;
public:
	static void Init(uint32_t amount);
	static void SetParticle(const Particle& p);
	static void Add(uint32_t amount);
	static void Add(const Particle& p);
	static void Draw(float deltaTime);

	ParticleSystem() {}

	inline static Particle& GetbaseParticle() { return s_BaseParticle; }
};
